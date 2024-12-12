#include <algorithm>
#include <vector>

using namespace std;

// 矩形面积并
// 离散化 + 扫描线 + 线段树
// 测试链接 : https://www.luogu.com.cn/problem/P5490
// @sa https://www.bilibili.com/video/BV16x4y1a7Ro/

class AreaSum
{
 public:
  static const int MAXN = 300001;

  vector<vector<int>> rec = vector<vector<int>>(MAXN, vector<int>(4));

  vector<vector<int>> line = vector<vector<int>>(MAXN, vector<int>(4));

  vector<int> ysort = vector<int>(MAXN);

  // 线段树某范围总长度
  vector<int> length = vector<int>(MAXN << 2);

  // 线段树某范围覆盖长度
  vector<int> cover = vector<int>(MAXN << 2);

  // 线段树某范围覆盖次数
  vector<int> times = vector<int>(MAXN << 2);

  int prepare(int n)
  {
    std::sort(ysort.begin() + 1, ysort.begin() + n + 1);
    int m = 1;
    for (int i = 2; i <= n; i++)
    {
      if (ysort[m] != ysort[i])
      {
        ysort[++m] = ysort[i];
      }
    }
    ysort[m + 1] = ysort[m];
    return m;
  }

  int rank(int n, int num)
  {
    int ans = 0;
    int l = 1, r = n, mid;
    while (l <= r)
    {
      mid = (l + r) >> 1;
      if (ysort[mid] >= num)
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
    length[i] = ysort[r + 1] - ysort[l];
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

  // 这个题的特殊性在于
  // 1) 查询操作永远查的是整个范围，不会有小范围的查询，每次都返回cover[1]
  // 2) 增加操作之后，后续一定会有等规模的减少操作
  // 根据以上两点分析出不需要懒更新机制
  // 首先当一次修改完成从下往上返回时，up方法能保证最上方的cover[1]是修改正确的
  // 同时任何一次增加操作所涉及的线段树范围，后续一定能被等规模的减少操作取消掉
  // 课上重点图解这个特殊性
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

  long compute(int n)
  {
    for (int i = 1, j = 1 + n, x1, y1, x2, y2; i <= n; i++, j++)
    {
      x1 = rec[i][0];
      y1 = rec[i][1];
      x2 = rec[i][2];
      y2 = rec[i][3];
      ysort[i] = y1;
      ysort[j] = y2;
      line[i][0] = x1;
      line[i][1] = y1;
      line[i][2] = y2;
      line[i][3] = 1;
      line[j][0] = x2;
      line[j][1] = y1;
      line[j][2] = y2;
      line[j][3] = -1;
    }
    n <<= 1;
    int m = prepare(n);
    build(1, m, 1);
    std::sort(line.begin() + 1, line.begin() + n + 1, [](auto& a, auto& b) { return a[0] < b[0]; });
    long ans = 0;
    for (int i = 1, pre = 0; i <= n; i++)
    {
      ans += (long) cover[1] * (line[i][0] - pre);
      pre = line[i][0];
      add(rank(m, line[i][1]), rank(m, line[i][2]) - 1, line[i][3], 1, m, 1);
    }
    return ans;
  }
};
