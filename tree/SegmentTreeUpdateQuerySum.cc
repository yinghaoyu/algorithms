#include <vector>

using namespace std;

// 线段树支持范围重置、范围查询
// 维护累加和

// @sa https://www.bilibili.com/video/BV1qm411k7yX/
class SegmentTreeUpdateQuerySum
{
 private:
  static const int MAXN = 100001;

  vector<long> arr = vector<long>(MAXN);

  vector<long> sum = vector<long>(MAXN << 2);

  vector<long> change = vector<long>(MAXN << 2);

  vector<bool> updating = vector<bool>(MAXN << 2);

 public:
  // 累加和信息的汇总
  void up(int i)
  {
    // 父范围的累加和 = 左范围累加和 + 右范围累加和
    sum[i] = sum[i << 1] + sum[i << 1 | 1];
  }

  // 懒信息的下发
  void down(int i, int ln, int rn)
  {
    if (updating[i])
    {
      // 发左
      lazy(i << 1, change[i], ln);
      // 发右
      lazy(i << 1 | 1, change[i], rn);
      // 父范围懒信息清空
      updating[i] = false;
    }
  }

  void lazy(int i, long v, int n)
  {
    sum[i] = v * n;  // 注意这里是重置，与累加不同
    change[i] = v;
    updating[i] = true;
  }

  // 建树
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
    change[i] = 0;
    updating[i] = 0;
  }

  // 范围修改
  void update(int jobl, int jobr, long jobv, int l, int r, int i)
  {
    if (jobl <= l && r <= jobr)
    {
      lazy(i, jobv, r - l + 1);
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

  // 查询累加和
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
