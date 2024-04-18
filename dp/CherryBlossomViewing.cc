#include <iostream>

using namespace std;

// 观赏樱花
// 给定一个背包的容量t，一共有n种货物，并且给定每种货物的信息
// 花费(cost)、价值(val)、数量(cnt)
// 如果cnt == 0，代表这种货物可以无限选择
// 如果cnt > 0，那么cnt代表这种货物的数量
// 挑选货物的总容量在不超过t的情况下，返回能得到的最大价值
// 背包容量不超过1000，每一种货物的花费都>0
// 测试链接 : https://www.luogu.com.cn/problem/P1833

// @sa https://www.bilibili.com/video/BV1Nz4y1c71M/
class CherryBlossomViewing
{
 public:
  static const int MAXN = 100001;
  static const int MAXW = 1001;
  static const int ENOUGH = 1001;
  static int v[MAXN];
  static int w[MAXN];
  static int dp[MAXW];
  static int hour1, minute1, hour2, minute2;
  static int t, n, m;

  void compute()
  {
    std::fill_n(dp, t + 1, 0);
    for (int i = 1; i <= m; i++)
    {
      for (int j = t; j >= w[i]; j--)
      {
        dp[j] = max(dp[j], dp[j - w[i]] + v[i]);
      }
    }
  }

  void solve()
  {
    while (cin >> hour1)
    {
      cin.ignore();  // 跳过冒号
      cin >> minute1;
      cin.ignore();  // 跳过冒号
      cin >> hour2;
      cin.ignore();  // 跳过冒号
      cin >> minute2;

      if (minute1 > minute2)
      {
        hour2--;
        minute2 += 60;
      }

      // 计算背包容量
      t = (hour2 - hour1) * 60 + minute2 - minute1;
      cin >> n;
      m = 0;
      for (int i = 0, cost, val, cnt; i < n; i++)
      {
        cin >> cost >> val >> cnt;
        if (cnt == 0)
        {
          // 关键点，本来是无限制的，但是题设有最大值
          // 补上足够的个数！！！
          cnt = ENOUGH;
        }
        // 二进制分组
        for (int k = 1; k <= cnt; k <<= 1)
        {
          v[++m] = k * val;
          w[m] = k * cost;
          cnt -= k;
        }
        if (cnt > 0)
        {
          v[++m] = cnt * val;
          w[m] = cnt * cost;
        }
      }
      compute();
      cout << dp[t] << endl;
    }
  }
};
