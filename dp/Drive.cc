#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

using namespace std;
using TdArray = vector<vector<int>>;

// 问题描述：
// 现有司机N*2人，调度中心将司机平分给A、B两个区域
// 第i个司机去A可得收入incode[i][0]
// 第i个司机去B可得收入incode[i][1]
// 返回所有调度方案中能使所有司机总收入最高的方案，是多少钱？

class Drive
{
 public:
  // income -> N * 2 的矩阵 N是偶数！
  // 0 [9, 13]
  // 1 [45,60]
  static int maxMoney1(TdArray &income)
  {
    if (income.size() < 2 || (income.size() & 1) != 0)
    {
      return 0;
    }
    int N = income.size();  // 司机数量一定是偶数，所以才能平分，A N /2 B N/2
    int M = N >> 1;         // M = N / 2 要去A区域的人
    return process1(income, 0, M);
  }

  // index.....所有的司机，往A和B区域分配！
  // A区域还有rest个名额!
  // 返回把index...司机，分配完，并且最终A和B区域同样多的情况下，index...这些司机，整体收入最大是多少！
  static int process1(TdArray &income, int index, int rest)
  {
    if (index == income.size())
    {
      return 0;
    }
    // 还剩下司机！
    if (income.size() - index == rest)
    {
      return income[index][0] + process1(income, index + 1, rest - 1);
    }
    if (rest == 0)
    {
      return income[index][1] + process1(income, index + 1, rest);
    }
    // 当前司机，可以去A，或者去B
    int p1 = income[index][0] + process1(income, index + 1, rest - 1);
    int p2 = income[index][1] + process1(income, index + 1, rest);
    return std::max(p1, p2);
  }

  // 严格位置依赖的动态规划版本
  static int maxMoney2(TdArray &income)
  {
    int N = income.size();
    int M = N >> 1;
    TdArray dp(N + 1, vector<int>(M + 1));
    for (int i = N - 1; i >= 0; i--)
    {
      for (int j = 0; j <= M; j++)
      {
        if (N - i == j)
        {
          dp[i][j] = income[i][0] + dp[i + 1][j - 1];
        }
        else if (j == 0)
        {
          dp[i][j] = income[i][1] + dp[i + 1][j];
        }
        else
        {
          int p1 = income[i][0] + dp[i + 1][j - 1];
          int p2 = income[i][1] + dp[i + 1][j];
          dp[i][j] = std::max(p1, p2);
        }
      }
    }
    return dp[0][M];
  }

  // 这题有贪心策略 :
  // 假设一共有10个司机，思路是先让所有司机去A，得到一个总收益
  // 然后看看哪5个司机改换门庭(去B)，可以获得最大的额外收益
  static int maxMoney3(TdArray &income)
  {
    int N = income.size();
    vector<int> arr(N);
    int sum = 0;
    for (int i = 0; i < N; i++)
    {
      arr[i] = income[i][1] - income[i][0];
      sum += income[i][0];
    }
    std::sort(arr.begin(), arr.end());
    int M = N >> 1;
    for (int i = N - 1; i >= M; i--)
    {
      sum += arr[i];
    }
    return sum;
  }

  // 返回随机len*2大小的正数矩阵
  // 值在0~value-1之间
  static TdArray randomMatrix(int len, int value)
  {
    TdArray ans(len << 1, vector<int>(2));
    for (int i = 0; i < ans.size(); i++)
    {
      ans[i][0] = getRandom(0, value);
      ans[i][1] = getRandom(0, value);
    }
    return ans;
  }

  static int getRandom(int min, int max)
  {
    random_device seed;                            // 硬件生成随机数种子
    ranlux48 engine(seed());                       // 利用种子生成随机数引
    uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
    int res = distrib(engine);                     // 随机数
    return res;
  }

  static void test()
  {
    int N = 10;
    int value = 100;
    int testTime = 500;
    cout << "测试开始" << endl;
    for (int i = 0; i < testTime; i++)
    {
      int len = getRandom(1, N);
      TdArray matrix = randomMatrix(len, value);
      int ans1 = maxMoney1(matrix);
      int ans2 = maxMoney2(matrix);
      int ans3 = maxMoney3(matrix);
      if (ans1 != ans2 || ans1 != ans3)
      {
        cout << ans1 << endl;
        cout << ans2 << endl;
        cout << ans3 << endl;
        cout << "Oops!" << endl;
      }
    }
    cout << "测试结束" << endl;
  }
};

int main()
{
  Drive::test();
  return 0;
}
