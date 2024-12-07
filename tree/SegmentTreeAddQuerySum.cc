#include <vector>

using namespace std;

// 线段树支持范围增加、范围查询
// 维护累加和

// @sa https://www.bilibili.com/video/BV1qm411k7yX/
class SegmentTreeAddQuerySum
{
 private:
  static const int MAXN = 100001;

  vector<long> arr = vector<long>(MAXN);

  vector<long> sum = vector<long>(MAXN << 2);

  vector<long> adding = vector<long>(MAXN << 2);

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
    if (adding[i] != 0)
    {
      // 发左
      lazy(i << 1, adding[i], ln);
      // 发右
      lazy(i << 1 | 1, adding[i], rn);
      // 父范围懒信息清空
      adding[i] = 0;
    }
  }

  // 当前来到l~r范围，对应的信息下标是i，范围上数字的个数是n = r-l+1
  // 现在收到一个懒更新任务 : l~r范围上每个数字增加v
  // 这个懒更新任务有可能是任务范围把当前线段树范围全覆盖导致的
  // 也有可能是父范围的懒信息下发下来的
  // 总之把线段树当前范围的sum数组和add数组调整好
  // 就不再继续往下下发了，懒住了
  void lazy(int i, long v, int n)
  {
    sum[i] += v * n;
    adding[i] += v;
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
    adding[i] = 0;
  }

  // 范围修改
  // jobl ~ jobr范围上每个数字增加jobv
  void add(int jobl, int jobr, long jobv, int l, int r, int i)
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
        add(jobl, jobr, jobv, l, mid, i << 1);
      }
      if (jobr > mid)
      {
        add(jobl, jobr, jobv, mid + 1, r, i << 1 | 1);
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
