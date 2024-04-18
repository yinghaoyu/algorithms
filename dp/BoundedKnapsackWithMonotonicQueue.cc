#include <vector>

using namespace std;

// 多重背包单调队列优化
// 宝物筛选
// 一共有n种货物, 背包容量为t
// 每种货物的价值(v[i])、重量(w[i])、数量(c[i])都给出
// 请返回选择货物不超过背包容量的情况下，能得到的最大的价值
// 测试链接 : https://www.luogu.com.cn/problem/P1776

// TODO: figure it out.
// @sa https://www.bilibili.com/video/BV1Nz4y1c71M/
class BoundedKnapsackWithMonotonicQueue
{
 public:
  static const int MAXN = 101;

  static const int MAXW = 40001;

  static int v[MAXN];

  static int w[MAXN];

  static int c[MAXN];

  static int dp[MAXW];

  static int queue[MAXW];

  static int l, r;

  static int n, t;

  int compute1()
  {
    vector<vector<int>> dp(n + 1, vector<int>(t + 1));
    for (int i = 1; i <= n; i++)
    {
      for (int mod = 0; mod <= std::min(t, w[i] - 1); mod++)
      {
        l = r = 0;
        // 根据余数分组
        for (int j = mod; j <= t; j += w[i])
        {
          // dp[i-1][j]
          // dp[i][j]
          // queue[r - 1] -> x
          // j -> y
          while (l < r && dp[i - 1][queue[r - 1]] + inc(j - queue[r - 1], i) <= dp[i - 1][j])
          {
            // queue[r-1]是队列尾部的列号 vs j这个列号
            // 指标之间pk
            r--;
          }
          queue[r++] = j;
          if (queue[l] == j - w[i] * (c[i] + 1))
          {
            // 检查单调队列最左的列号，是否过期
            // 比如
            // i号物品，重量为3，个数4
            // queue[l]是队列头部的列号，假设是2
            // 当j == 17时，依赖的格子为dp[i-1][17、14、11、8、5]
            // 所以此时头部的列号2，过期了，要弹出
            l++;
          }
          // dp[i][j] = dp[i-1][拥有最强指标的列] + (j - 拥有最强指标的列) / i号物品重量 * i号物品价值
          dp[i][j] = dp[i - 1][queue[l]] + inc(j - queue[l], i);
        }
      }
    }
    return dp[n][t];
  }

  // s的容量用来装i号商品，可以得到多少价值
  int inc(int s, int i) { return s / w[i] * v[i]; }

  // 单调队列优化枚举 + 空间压缩
  // 理解了原理之后，这个函数就没有理解难度了
  // 难度来自实现和注意边界条件，可以自己尝试一下
  int compute2()
  {
    for (int i = 1; i <= n; i++)
    {
      for (int mod = 0; mod <= std::min(t, w[i] - 1); mod++)
      {
        // 因为空间压缩时，关于j的遍历是从右往左，而不是从左往右
        // 所以先让dp[i-1][...右侧的几个位置]进入窗口
        l = r = 0;
        for (int j = t - mod, k = 0; j >= 0 && k <= c[i]; j -= w[i], k++)
        {
          while (l < r && dp[j] + inc(queue[r - 1] - j, i) >= dp[queue[r - 1]])
          {
            r--;
          }
          queue[r++] = j;
        }
        // 然后j开始从右往左遍历
        // 注意，因为j从右往左遍历，所以：
        // 更靠右的j位置更早进入窗口
        // 更靠左的j位置更晚进入窗口
        for (int j = t - mod; j >= 0; j -= w[i])
        {
          // 来到j，计算dp[i][j]的值，做了空间压缩，所以去更新dp[j]
          dp[j] = dp[queue[l]] + inc(j - queue[l], i);
          // 求解完dp[j]
          // 接下来要去求解dp[j-w[i]]了(根据余数分组)
          // 所以看看窗口最左的下标是不是j(其实代表dp[i-1][j]的值]
          // 是的话，说明最左下标过期了，要弹出
          if (queue[l] == j)
          {
            l++;
          }
          // 最后
          // 因为接下来要去求解dp[j-w[i]]了
          // 所以新的dp[i-1][enter]要进入窗口了
          // 用单调队列的更新方式让其进入
          int enter = j - w[i] * (c[i] + 1);
          if (enter >= 0)
          {
            while (l < r && dp[enter] + inc(queue[r - 1] - enter, i) >= dp[queue[r - 1]])
            {
              r--;
            }
            queue[r++] = enter;
          }
        }
      }
    }
    return dp[t];
  }
};
