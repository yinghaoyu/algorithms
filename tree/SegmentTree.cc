#include <iostream>
#include <random>
#include <vector>

using namespace std;

int getRandom(int min, int max)
{
  random_device seed;                            // 硬件生成随机数种子
  ranlux48 engine(seed());                       // 利用种子生成随机数引
  uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
  int res = distrib(engine);                     // 随机数
  return res;
}

class SegmentTree
{
 private:
  int MAXN;
  vector<int> arr;      // 原序列的信息从0开始，但在arr里是从1开始的
  vector<int> sum;      // 模拟线段树维护区间和
  vector<int> lazy;     // 累加和懒惰标记
  vector<int> change;   // 更新的值
  vector<bool> update;  // 更新慵懒标记，这个数组不能省，因为update的值如果是0，也算update操作
 public:
  SegmentTree(vector<int> &origin)
  {
    MAXN = origin.size() + 1;
    arr = vector<int>(MAXN);  // arr[0] 不用 从1开始使用，这样左孩子就是2*i，右孩子是2*i + 1，方便用位运算
    for (int i = 1; i < MAXN; i++)
    {
      arr[i] = origin[i - 1];
    }
    // 假设叶子节点为N(N满足N = 2^n，这里假设N = 8)个，刚好构成一颗左右子树可以平均分配的满二叉树，
    // 对于满二叉树，叶子节点N = 8个，那么非叶子节点就有 N - 1 = 7个，总共节点数就有 2N - 1 = 15 个
    // 2N = 16 个位置绝对能覆盖 2N - 1 = 15

    // 对于最差的情况，叶子结点有 N = 2^n + 1个，假设 N = 9 个，那么为了构成一颗左右子树可以平均分配的满二叉树，
    // 就需要补充节点值为0的节点，使得 叶子结点数为 2^(n + 1) = 16个，我们取 M = 2N = 18，加上 2N - 1 个非叶子节点
    // 也就是说 4N绝对能覆盖所有情况
    int QMAXN = MAXN << 2;
    sum = vector<int>(QMAXN);      // 用来支持脑补概念中，某一个范围的累加和信息
    lazy = vector<int>(QMAXN);     // 用来支持脑补概念中，某一个范围沒有往下传递的累加任务
    change = vector<int>(QMAXN);   // 用来支持脑补概念中，某一个范围有没有更新操作的任务
    update = vector<bool>(QMAXN);  // 用来支持脑补概念中，某一个范围更新任务，更新成了什么
  }

  // 汇总
  void pushUp(int rt) { sum[rt] = sum[rt << 1] + sum[rt << 1 | 1]; }

  // 之前的，所有懒增加，和懒更新，从父范围，发给左右两个子范围
  // 分发策略是什么
  // ln表示左子树元素结点个数，rn表示右子树结点个数
  void pushDown(int rt, int ln, int rn)
  {
    // 必须先发lazy update任务
    if (update[rt])
    {
      // 如果父节点有lazy update任务，那么一定比子节点要晚，所以采取覆盖方式用=，而不是+=
      update[rt << 1] = true;
      update[rt << 1 | 1] = true;
      change[rt << 1] = change[rt];      // 往左孩子下发lazy update
      change[rt << 1 | 1] = change[rt];  // 往右孩子下发lazy update
      lazy[rt << 1] = 0;
      lazy[rt << 1 | 1] = 0;
      sum[rt << 1] = change[rt] * ln;
      sum[rt << 1 | 1] = change[rt] * rn;
      update[rt] = false;
    }
    // 在lazy update任务之后，之前lazy add任务积累的lazy[rt]会被清 0
    // 这时如果lazy[rt]不为0，说明在lazy update任务之后，又有新的lazy add任务
    // 如果有lazy add任务，那么继续下发lazy add任务
    if (lazy[rt] != 0)
    {
      // 把lazy add任务下发，同时左、右孩子也懒更新
      lazy[rt << 1] += lazy[rt];  // 下发到左孩子
      sum[rt << 1] += lazy[rt] * ln;
      lazy[rt << 1 | 1] += lazy[rt];  // 下发到右孩子
      sum[rt << 1 | 1] += lazy[rt] * rn;
      lazy[rt] = 0;  // 清空当前
    }
  }

  // 在初始化阶段，先把sum数组，填好
  // 在arr[l~r]范围上，去build，1~N，
  // rt : 这个范围在sum中的下标
  void build(int l, int r, int rt)
  {
    if (l == r)
    {
      sum[rt] = arr[l];
      return;
    }
    int mid = (l + r) >> 1;
    build(l, mid, rt << 1);
    build(mid + 1, r, rt << 1 | 1);
    pushUp(rt);
  }

  // L~R  所有的值变成C
  // l~r  rt
  void updates(int L, int R, int C, int l, int r, int rt)
  {
    // 任务如果把[l, r]全包了！
    if (L <= l && r <= R)
    {
      update[rt] = true;
      change[rt] = C;             // update的懒任务
      sum[rt] = C * (r - l + 1);  // 注意这里不是累加
      lazy[rt] = 0;               // 取消旧的lazy add任务
      return;
    }
    // 当前任务躲不掉，无法懒更新，要往下发
    int mid = (l + r) >> 1;
    pushDown(rt, mid - l + 1, r - mid);  // 如果有旧的lazy update，先把旧的lazy update下发
    if (L <= mid)
    {
      updates(L, R, C, l, mid, rt << 1);
    }
    if (R > mid)
    {
      updates(L, R, C, mid + 1, r, rt << 1 | 1);
    }
    pushUp(rt);
  }

  // L~R, C 任务！ 表示在区间[L, R] 加上 C
  // l~r, rt      表示当前所在的区间[l, r]，以及在sum数组的位置rt
  void add(int L, int R, int C, int l, int r, int rt)
  {
    // 任务如果把[l, r]全包了！
    if (L <= l && r <= R)
    {
      // 懒住
      // 这里就是线段树的优势，常规方案累加，时间复杂度要O(N)
      // 而懒更新累加，时间复杂度为O(log(N))
      // 如果线段树不用懒更新，每次累加就需要更新所有节点(包括非叶子节点)，反而增加了负担
      sum[rt] += C * (r - l + 1);  // 直接更新根节点的sum，区间数*值
      lazy[rt] += C;               // 懒更新，注意这里是累加
      return;
    }
    // 任务没有把[l ,r]全包住
    // 就要把任务下发
    int mid = (l + r) >> 1;
    // 下发之前的lazy add任务
    pushDown(rt, mid - l + 1, r - mid);
    // 左孩子是否需要接到任务
    if (L <= mid)
    {
      add(L, R, C, l, mid, rt << 1);
    }
    // 右孩子是否需要接到任务
    if (R > mid)
    {
      add(L, R, C, mid + 1, r, rt << 1 | 1);
    }
    // 左右孩子做完任务后，更新我的sum信息
    pushUp(rt);
  }

  // 1~6 累加和是多少？ 1~8 rt
  long query(int L, int R, int l, int r, int rt)
  {
    if (L <= l && r <= R)
    {
      return sum[rt];
    }
    int mid = (l + r) >> 1;
    pushDown(rt, mid - l + 1, r - mid);
    long ans = 0;
    if (L <= mid)
    {
      ans += query(L, R, l, mid, rt << 1);
    }
    if (R > mid)
    {
      ans += query(L, R, mid + 1, r, rt << 1 | 1);
    }
    return ans;
  }
};

class Right  // 测试用的前缀和
{
 public:
  vector<int> arr;

  Right(vector<int> &origin)
  {
    arr = vector<int>(origin.size() + 1);
    for (int i = 0; i < origin.size(); i++)
    {
      arr[i + 1] = origin[i];
    }
  }

  void update(int L, int R, int C)
  {
    for (int i = L; i <= R; i++)
    {
      arr[i] = C;
    }
  }

  void add(int L, int R, int C)
  {
    for (int i = L; i <= R; i++)
    {
      arr[i] += C;
    }
  }

  long query(int L, int R)
  {
    long ans = 0;
    for (int i = L; i <= R; i++)
    {
      ans += arr[i];
    }
    return ans;
  }
};

vector<int> genarateRandomArray(int len, int max)
{
  int size = getRandom(1, len);
  vector<int> origin(size);
  for (int i = 0; i < size; i++)
  {
    origin[i] = getRandom(0, max) - getRandom(0, max);
  }
  return origin;
}

bool test()
{
  int len = 100;
  int max = 1000;
  int testTimes = 5000;
  int addOrUpdateTimes = 1000;
  int queryTimes = 500;
  for (int i = 0; i < testTimes; i++)
  {
    vector<int> origin = genarateRandomArray(len, max);
    SegmentTree seg(origin);
    int S = 1;
    int N = origin.size();  // N >= 1
    int root = 1;
    seg.build(S, N, root);
    Right rig(origin);
    for (int j = 0; j < addOrUpdateTimes; j++)
    {
      int num1 = getRandom(1, N);
      int num2 = getRandom(1, N);
      int L = std::min(num1, num2);
      int R = std::max(num1, num2);
      int C = getRandom(0, max) - getRandom(0, max);
      if (getRandom(0, max) < 0.5 * max)
      {
        seg.add(L, R, C, S, N, root);
        rig.add(L, R, C);
      }
      else
      {
        seg.updates(L, R, C, S, N, root);
        rig.update(L, R, C);
      }
    }
    for (int k = 0; k < queryTimes; k++)
    {
      int num1 = getRandom(1, N);
      int num2 = getRandom(1, N);
      int L = std::min(num1, num2);
      int R = std::max(num1, num2);
      long ans1 = seg.query(L, R, S, N, root);
      long ans2 = rig.query(L, R);
      if (ans1 != ans2)
      {
        cout << L << " " << R << " " << N << " " << ans1 << " " << ans2 << endl;
        return false;
      }
    }
  }
  return true;
}

int main()
{
  vector<int> origin = {2, 1, 1, 2, 3, 4, 5};
  SegmentTree seg(origin);
  int S = 1;     // 整个区间的开始位置，规定从1开始，不从0开始 -> 固定
  int N = 7;     // 整个区间的结束位置，规定能到N，不是N-1 -> 固定
  int root = 1;  // 整棵树的头节点位置，规定是1，不是0 -> 固定
  int L = 2;     // 操作区间的开始位置 -> 可变
  int R = 5;     // 操作区间的结束位置 -> 可变
  int C = 4;     // 要加的数字或者要更新的数字 -> 可变
  // 区间生成，必须在[S,N]整个范围上build
  seg.build(S, N, root);
  // 区间修改，可以改变L、R和C的值，其他值不可改变
  seg.add(L, R, C, S, N, root);
  // 区间更新，可以改变L、R和C的值，其他值不可改变
  seg.updates(L, R, C, S, N, root);
  // 区间查询，可以改变L和R的值，其他值不可改变
  long sum = seg.query(L, R, S, N, root);
  cout << sum << endl;

  cout << "对数器测试开始..." << endl;
  cout << "测试结果 : " << (test() ? "通过" : "未通过") << endl;
  return 0;
}
