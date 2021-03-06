#include <iostream>
#include <memory.h>
#include <stdbool.h>
#include <random>

using namespace std;

int getRandom(int min, int max)
{
  random_device seed;  // 硬件生成随机数种子
  ranlux48 engine(seed());  // 利用种子生成随机数引
  uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
  int res = distrib(engine);  // 随机数
  return res;
}

class SegmentTree
{
  private:
    int MAXN;
    int* arr;  // 原序列的信息从0开始，但在arr里是从1开始的
    int* sum;  // 模拟线段树维护区间和
    int* lazy;  // 累加和懒惰标记
    int* change;  // 更新的值
    bool* update;  // 更新慵懒标记
  public:
    SegmentTree(int origin[], int len)
    {
      MAXN = len + 1;
      arr = new int[MAXN]; // arr[0] 不用 从1开始使用
      for (int i = 1; i < MAXN; i++)
      {
        arr[i] = origin[i - 1];
      }
      int QMAXN = MAXN << 2;
      sum = new int[QMAXN]; // 用来支持脑补概念中，某一个范围的累加和信息
      lazy = new int[QMAXN]; // 用来支持脑补概念中，某一个范围沒有往下传递的累加任务
      change = new int[QMAXN]; // 用来支持脑补概念中，某一个范围有没有更新操作的任务
      update = new bool[QMAXN]; // 用来支持脑补概念中，某一个范围更新任务，更新成了什么
      for(int i = 0; i < QMAXN; ++i)
      {
        sum[i] = 0;
      }
      for(int i = 0; i < QMAXN; ++i)
      {
        lazy[i] = 0;
      }
      for(int i = 0; i < QMAXN; ++i)
      {
        change[i] = 0;
      }
      for(int i = 0; i < QMAXN; ++i)
      {
        update[i] = 0;
      }
    }

    void pushUp(int rt)
    {
      sum[rt] = sum[rt << 1] + sum[rt << 1 | 1];
    }
    // 之前的，所有懒增加，和懒更新，从父范围，发给左右两个子范围
    // 分发策略是什么
    // ln表示左子树元素结点个数，rn表示右子树结点个数
    void pushDown(int rt, int ln, int rn)
    {
      if (update[rt])
      {
        update[rt << 1] = true;
        update[rt << 1 | 1] = true;
        change[rt << 1] = change[rt];
        change[rt << 1 | 1] = change[rt];
        lazy[rt << 1] = 0;
        lazy[rt << 1 | 1] = 0;
        sum[rt << 1] = change[rt] * ln;
        sum[rt << 1 | 1] = change[rt] * rn;
        update[rt] = false;
      }
      if (lazy[rt] != 0)
      {
        lazy[rt << 1] += lazy[rt];
        sum[rt << 1] += lazy[rt] * ln;
        lazy[rt << 1 | 1] += lazy[rt];
        sum[rt << 1 | 1] += lazy[rt] * rn;
        lazy[rt] = 0;
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
    void updates(int L, int R, int C, int l, int r, int rt) {
      if (L <= l && r <= R)
      {
        update[rt] = true;
        change[rt] = C;
        sum[rt] = C * (r - l + 1);
        lazy[rt] = 0;
        return;
      }
      // 当前任务躲不掉，无法懒更新，要往下发
      int mid = (l + r) >> 1;
      pushDown(rt, mid - l + 1, r - mid);
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
    // L~R, C 任务！
    // rt，l~r
    void add(int L, int R, int C, int l, int r, int rt)
    {
      // 任务如果把此时的范围全包了！
      if (L <= l && r <= R)
      {
        sum[rt] += C * (r - l + 1);  // 直接更新根节点的sum，区间数*值
        lazy[rt] += C;  // 懒更新
        return;
      }
      // 任务没有把你全包！
      // l  r  mid = (l+r)/2
      int mid = (l + r) >> 1;
      pushDown(rt, mid - l + 1, r - mid);
      // L~R
      if (L <= mid)
      {
        add(L, R, C, l, mid, rt << 1);
      }
      if (R > mid)
      {
        add(L, R, C, mid + 1, r, rt << 1 | 1);
      }
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
    int* arr;

    Right(int origin[], int len)
    {
      arr = new int[len + 1];
      for (int i = 0; i < len; i++)
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

int* genarateRandomArray(int len, int max)
{
  int size = getRandom(0, len) + 1;
  int* origin = new int[size];
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
    int* origin = genarateRandomArray(len, max);
    SegmentTree seg(origin, len);
    int S = 1;
    int N = len;
    int root = 1;
    seg.build(S, N, root);
    Right rig(origin, len);
    for (int j = 0; j < addOrUpdateTimes; j++)
    {
      int num1 = getRandom(0, N) + 1;
      int num2 = getRandom(0, N) + 1;
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
  int origin[] = { 2, 1, 1, 2, 3, 4, 5 };
  SegmentTree seg(origin, 7);
  int S = 1; // 整个区间的开始位置，规定从1开始，不从0开始 -> 固定
  int N = 7; // 整个区间的结束位置，规定能到N，不是N-1 -> 固定
  int root = 1; // 整棵树的头节点位置，规定是1，不是0 -> 固定
  int L = 2; // 操作区间的开始位置 -> 可变
  int R = 5; // 操作区间的结束位置 -> 可变
  int C = 4; // 要加的数字或者要更新的数字 -> 可变
  // 区间生成，必须在[S,N]整个范围上build
  seg.build(S, N, root);
  // 区间修改，可以改变L、R和C的值，其他值不可改变
  seg.add(L, R, C, S, N, root);
  // 区间更新，可以改变L、R和C的值，其他值不可改变
  seg.updates(L, R, C, S, N, root);
  // 区间查询，可以改变L和R的值，其他值不可改变
  long sum = seg.query(L, R, S, N, root);
  cout<< sum <<endl;

  cout << "对数器测试开始..." << endl;
  cout << "测试结果 : " << (test() ? "通过" : "未通过") << endl;
  return 0;
}
