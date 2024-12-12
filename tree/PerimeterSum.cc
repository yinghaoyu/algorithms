#include <algorithm>
#include <vector>

using namespace std;

// 矩形周长并(洛谷测试)
// 离散化 + 线段树 + 扫描线
// 测试链接 : https://www.luogu.com.cn/problem/P1856
// @sa https://www.bilibili.com/video/BV16x4y1a7Ro/

class PerimeterSum
{
 public:
  static const int MAXN = 20001;

  vector<vector<int>> rec = vector<vector<int>>(MAXN, vector<int>(4));

  vector<vector<int>> line = vector<vector<int>>(MAXN, vector<int>(4));

  vector<int> vsort = vector<int>(MAXN);

  // 线段树某范围总长度
  vector<int> length = vector<int>(MAXN << 2);

  // 线段树某范围覆盖长度
  vector<int> cover = vector<int>(MAXN << 2);

  // 线段树某范围覆盖次数
  vector<int> times = vector<int>(MAXN << 2);

  int prepare(int n)
  {
    std::sort(vsort.begin() + 1, vsort.end() + n + 1);
    int m = 1;
    for (int i = 2; i <= n; i++)
    {
      if (vsort[m] != vsort[i])
      {
        vsort[++m] = vsort[i];
      }
    }
    vsort[m + 1] = vsort[m];
    return m;
  }

  int rank(int n, int num)
  {
    int ans = 0;
    int l = 1, r = n, mid;
    while (l <= r)
    {
      mid = (l + r) >> 1;
      if (vsort[mid] >= num)
      {
        ans = mid;
        r = mid - 1;
      }
      else
      {
        l = mid + 1;
      }
    }
    return ans;
  }

  void build(int l, int r, int i)
  {
    if (l < r)
    {
      int mid = (l + r) >> 1;
      build(l, mid, i << 1);
      build(mid + 1, r, i << 1 | 1);
    }
    length[i] = vsort[r + 1] - vsort[l];
    times[i] = 0;
    cover[i] = 0;
  }

  void up(int i)
  {
    if (times[i] > 0)
    {
      cover[i] = length[i];
    }
    else
    {
      cover[i] = cover[i << 1] + cover[i << 1 | 1];
    }
  }

  void add(int jobl, int jobr, int jobv, int l, int r, int i)
  {
    if (jobl <= l && r <= jobr)
    {
      times[i] += jobv;
    }
    else
    {
      int mid = (l + r) >> 1;
      if (jobl <= mid)
      {
        add(jobl, jobr, jobv, l, mid, i << 1);
      }
      if (jobr > mid)
      {
        add(jobl, jobr, jobv, mid + 1, r, i << 1 | 1);
      }
    }
    up(i);
  }

  long compute(int n) { return scanY(n) + scanX(n); }

  long scanY(int n)
  {
    for (int i = 1, j = 1 + n, x1, y1, x2, y2; i <= n; i++, j++)
    {
      x1 = rec[i][0];
      y1 = rec[i][1];
      x2 = rec[i][2];
      y2 = rec[i][3];
      vsort[i] = y1;
      vsort[j] = y2;
      line[i][0] = x1;
      line[i][1] = y1;
      line[i][2] = y2;
      line[i][3] = 1;
      line[j][0] = x2;
      line[j][1] = y1;
      line[j][2] = y2;
      line[j][3] = -1;
    }
    return scan(n << 1);
  }

  long scanX(int n)
  {
    for (int i = 1, j = 1 + n, x1, y1, x2, y2; i <= n; i++, j++)
    {
      x1 = rec[i][0];
      y1 = rec[i][1];
      x2 = rec[i][2];
      y2 = rec[i][3];
      vsort[i] = x1;
      vsort[j] = x2;
      line[i][0] = y1;
      line[i][1] = x1;
      line[i][2] = x2;
      line[i][3] = 1;
      line[j][0] = y2;
      line[j][1] = x1;
      line[j][2] = x2;
      line[j][3] = -1;
    }
    return scan(n << 1);
  }

  long scan(int n)
  {
    int m = prepare(n);
    build(1, m, 1);
    // 这里有个坑
    // 在排序时，如果同一个位置的扫描线有多条，也就是a[0] == b[0]时
    // 应该先处理区间覆盖+1的扫描线，然后再处理区间覆盖-1的扫描线
    // 不然投影长度会频繁变化，导致答案错误
    // 不过测试数据并没有安排这方面的测试
    std::sort(line.begin() + 1, line.end() + n + 1, [](auto& a, auto& b) { return a[0] != b[0] ? (a[0] < b[0]) : (b[3] < a[3]); });
    long ans = 0;
    for (int i = 1, pre; i <= n; i++)
    {
      pre = cover[1];
      add(rank(m, line[i][1]), rank(m, line[i][2]) - 1, line[i][3], 1, m, 1);
      ans += std::abs(cover[1] - pre);
    }
    return ans;
  }
};
