#include <algorithm>
#include <climits>
#include <vector>

using namespace std;

// 线段树同时支持范围重置、范围增加、范围查询
// 维护最大值

// @sa https://www.bilibili.com/video/BV12i421X76h/

class SegmentTreeUpdateAddQuerySum
{
 private:
  static const int MAXN = 1000001;

  vector<long> arr = vector<long>(MAXN);

  vector<long> max = vector<long>(MAXN);

  vector<long> adding = vector<long>(MAXN);

  vector<long> change = vector<long>(MAXN);

  vector<bool> updating = vector<bool>(MAXN);

 public:
  void up(int i) { max[i] = std::max(max[i << 1], max[i << 1 | 1]); }

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
    max[i] = v;
    adding[i] = 0;
    change[i] = v;
    updating[i] = true;
  }

  void addLazy(int i, long v, int n)
  {
    max[i] += v;
    adding[i] += v;
  }

  void build(int l, int r, int i)
  {
    if (l == r)
    {
      max[i] = arr[l];
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
      return max[i];
    }
    int mid = (l + r) >> 1;
    down(i, mid - l + 1, r - mid);
    long ans = LONG_MIN;
    if (jobl <= mid)
    {
      ans = std::max(ans, query(jobl, jobr, l, mid, i << 1));
    }
    if (jobr > mid)
    {
      ans = std::max(ans, query(jobl, jobr, mid + 1, r, i << 1 | 1));
    }
    return ans;
  }
};
