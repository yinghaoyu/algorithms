#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

// 线段树同时支持范围重置、范围增加、范围查询
// 维护累加和
// 验证

// @sa https://www.bilibili.com/video/BV12i421X76h/

class SegmentTreeUpdateAddQuerySum
{
 private:
  static const int MAXN = 1000001;

  vector<long> arr = vector<long>(MAXN);

  vector<long> sum = vector<long>(MAXN);

  vector<long> adding = vector<long>(MAXN);

  vector<long> change = vector<long>(MAXN);

  vector<bool> updating = vector<bool>(MAXN);

 public:
  SegmentTreeUpdateAddQuerySum(vector<long>& t) { std::copy(t.begin(), t.end(), arr.begin()); }

  void up(int i) { sum[i] = sum[i << 1] + sum[i << 1 | 1]; }

  void down(int i, int ln, int rn)
  {
    // 先update，再add
    // 因为先add，再update，add的操作就没有意义
    if (updating[i])
    {
      updateLazy(i << 1, change[i], ln);
      updateLazy(i << 1 | 1, change[i], rn);
      updating[i] = false;
    }
    if (adding[i] != 0)
    {
      addLazy(i << 1, adding[i], ln);
      addLazy(i << 1 | 1, adding[i], rn);
      adding[i] = 0;
    }
  }

  void updateLazy(int i, long v, int n)
  {
    sum[i] = v * n;
    adding[i] = 0;
    change[i] = v;
    updating[i] = true;
  }

  void addLazy(int i, long v, int n)
  {
    sum[i] += v * n;
    adding[i] += v;
  }

  void build(int l, int r, int i)
  {
    if (l == r)
    {
      sum[i] = arr[l];
    }
    else
    {
      int mid = (l + r) >> 1;
      build(l, mid, i << 1);
      build(mid + 1, r, i << 1 | 1);
      up(i);
    }
    adding[i] = 0;
    change[i] = 0;
    updating[i] = false;
  }

  void update(int jobl, int jobr, long jobv, int l, int r, int i)
  {
    if (jobl <= l && r <= jobr)
    {
      updateLazy(i, jobv, r - l + 1);
    }
    else
    {
      int mid = (l + r) >> 1;
      down(i, mid - l + 1, r - mid);
      if (jobl <= mid)
      {
        update(jobl, jobr, jobv, l, mid, i << 1);
      }
      if (jobr > mid)
      {
        update(jobl, jobr, jobv, mid + 1, r, i << 1 | 1);
      }
      up(i);
    }
  }

  void add(int jobl, int jobr, long jobv, int l, int r, int i)
  {
    if (jobl <= l && r <= jobr)
    {
      addLazy(i, jobv, r - l + 1);
    }
    else
    {
      int mid = (l + r) >> 1;
      down(i, mid - l + 1, r - mid);
      if (jobl <= mid)
      {
        add(jobl, jobr, jobv, l, mid, i << 1);
      }
      if (jobr > mid)
      {
        add(jobl, jobr, jobv, mid + 1, r, i << 1 | 1);
      }
      up(i);
    }
  }

  long query(int jobl, int jobr, int l, int r, int i)
  {
    if (jobl <= l && r <= jobr)
    {
      return sum[i];
    }
    int mid = (l + r) >> 1;
    down(i, mid - l + 1, r - mid);
    long ans = 0;
    if (jobl <= mid)
    {
      ans += query(jobl, jobr, l, mid, i << 1);
    }
    if (jobr > mid)
    {
      ans += query(jobl, jobr, mid + 1, r, i << 1 | 1);
    }
    return ans;
  }
};

int getRandom(int min, int max)
{
  random_device seed;                            // 硬件生成随机数种子
  ranlux48 engine(seed());                       // 利用种子生成随机数引
  uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
  int res = distrib(engine);                     // 随机数
  return res;
}

// 生成随机值填入arr数组
// 为了验证
void randomArray(vector<long>& arr, int n, int v)
{
  for (int i = 1; i <= n; i++)
  {
    arr[i] = getRandom(0, v);
  }
}

// 验证结构的增加
// 暴力增加
// 为了验证
void checkAdd(vector<long>& check, int jobl, int jobr, long jobv)
{
  for (int i = jobl; i <= jobr; i++)
  {
    check[i] += jobv;
  }
}

// 验证结构的更新
// 暴力更新
// 为了验证
void checkUpdate(vector<long>& check, int jobl, int jobr, long jobv)
{
  for (int i = jobl; i <= jobr; i++)
  {
    check[i] = jobv;
  }
}

// 验证结构的查询
// 暴力查询
// 为了验证
long checkQuery(vector<long>& check, int jobl, int jobr)
{
  long ans = 0;
  for (int i = jobl; i <= jobr; i++)
  {
    ans += check[i];
  }
  return ans;
}

int main()
{
  cout << "测试开始" << endl;
  int n = 1000;
  int v = 2000;
  int t = 5000000;

  // 生成验证的结构
  vector<long> check = vector<long>(n + 1);

  // 生成随机值填入check数组
  randomArray(check, n, v);

  SegmentTreeUpdateAddQuerySum s(check);

  // 建立线段树
  s.build(1, n, 1);
  for (int i = 1; i <= t; i++)
  {
    // 生成操作类型
    // op = 0 增加操作
    // op = 1 更新操作
    // op = 2 查询操作
    int op = getRandom(0, 2);
    // 下标从1开始，不从0开始，生成两个随机下标
    int a = getRandom(1, n - 1);
    int b = getRandom(1, n - 1);
    // 确保jobl <= jobr
    int jobl = std::min(a, b);
    int jobr = std::max(a, b);
    if (op == 0)
    {
      // 增加操作
      // 线段树、验证结构同步增加
      int jobv = getRandom(-v, v);
      s.add(jobl, jobr, jobv, 1, n, 1);
      checkAdd(check, jobl, jobr, jobv);
    }
    else if (op == 1)
    {
      // 更新操作
      // 线段树、验证结构同步更新
      int jobv = getRandom(-v, v);
      s.update(jobl, jobr, jobv, 1, n, 1);
      checkUpdate(check, jobl, jobr, jobv);
    }
    else
    {
      // 查询操作
      // 线段树、验证结构同步查询
      // 比对答案
      long ans1 = s.query(jobl, jobr, 1, n, 1);
      long ans2 = checkQuery(check, jobl, jobr);
      if (ans1 != ans2)
      {
        cout << "出错了!" << endl;
      }
    }
  }
  cout << "测试结束" << endl;
}
