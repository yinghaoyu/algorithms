#include <iostream>
#include <random>
#include <vector>

using namespace std;

using TdArray = vector<vector<int>>;

// 问题描述：有N堆石子，现要将石子有次序地合并成一堆。
// 规定每次只能选相邻的2堆石子合并成新的一堆，并将新的一堆石子数记为该次合并的得分。
// 请计算出将N堆石子合并成一堆的最小得分。

// 例如：每堆石子的数量分别是3，4，11。
// 有两种选择：
// 1、前两堆石头合并再和第三堆石头合并。
//    3+4=7 --> 7 石堆变为（7， 11）
//    7+11=18 --> 18 石堆变为（18）
//    cost = 7 + 18 = 25
// 2、后两堆石头合并再和第一堆石头合并。
//    4 + 11 = 15 --> 15 石堆变为（3，15）
//    3 + 15 = 18 --> 18 石堆变为（18）
//    cost = 15 + 18 = 33
// 那么cost的最小值为25

// 关于四边形不等式优化dp：https://zhuanlan.zhihu.com/p/398419302
class StoneMerge
{
 public:
  // 计算前缀和
  static vector<int> sum(vector<int> &arr)
  {
    int N = arr.size();
    vector<int> s(N + 1);
    s[0] = 0;
    for (int i = 0; i < N; i++)
    {
      s[i + 1] = s[i] + arr[i];
    }
    return s;
  }

  // 返回arr[l..r]的区间和
  static int w(vector<int> &s, int l, int r) { return s[r + 1] - s[l]; }

  static int min1(vector<int> &arr)
  {
    if (arr.size() < 2)
    {
      return 0;
    }
    vector<int> s = sum(arr);
    return process1(0, arr.size() - 1, s);
  }

  static int process1(int L, int R, vector<int> &s)
  {
    if (L == R)
    {
      // 只剩一个元素，不用合并cost为0
      return 0;
    }
    int next = INT32_MAX;
    // 以leftEnd为分界线点划分
    for (int leftEnd = L; leftEnd < R; leftEnd++)
    {
      // 这里的 +1 保证了一定是取相邻的石子合并
      next = std::min(next, process1(L, leftEnd, s) + process1(leftEnd + 1, R, s));
    }
    // next为子过程的结果，加上当前合并的结果
    return next + w(s, L, R);
  }

  // arr[4] = {3, 4, 11, 9}
  // sum[5] = {0, 3, 7, 18, 27}
  //
  //     0  1  2  3 R             0  1  2  3  R
  //  0  0  ?  ?  ♠            0  0  7  ?  ♠
  //  1  ×  0  ?  ?            1  ×  0  15 39
  //  2  ×  ×  0  ?      -->   2  ×  ×  0  20
  //  3  ×  ×  ×  0            3  ×  ×  ×  0
  //  L                        L
  static int min2(vector<int> &arr)
  {
    if (arr.size() < 2)
    {
      return 0;
    }
    int N = arr.size();
    vector<int> s = sum(arr);
    TdArray dp(N, vector<int>(N));
    // dp[i][i] = 0 // base case
    // 根据递归的状态从下往上，从左往右填
    for (int L = N - 2; L >= 0; L--)
    {
      for (int R = L + 1; R < N; R++)
      {
        int next = INT32_MAX;
        // dp(L..leftEnd)  + dp[leftEnd+1...R]  + 累加和[L...R]
        for (int leftEnd = L; leftEnd < R; leftEnd++)
        {
          next = std::min(next, dp[L][leftEnd] + dp[leftEnd + 1][R]);
        }
        dp[L][R] = next + w(s, L, R);
      }
    }
    return dp[0][N - 1];
  }

  // 四边形不等式优化dp
  static int min3(vector<int> &arr)
  {
    if (arr.size() < 2)
    {
      return 0;
    }
    int N = arr.size();
    vector<int> s = sum(arr);
    TdArray dp(N, vector<int>(N));
    // 得到dp[L][R]的最优划分点记录在best里
    TdArray best(N, vector<int>(N));
    for (int i = 0; i < N - 1; i++)
    {
      best[i][i + 1] = i;
      dp[i][i + 1] = w(s, i, i + 1);
    }
    for (int L = N - 3; L >= 0; L--)
    {
      for (int R = L + 2; R < N; R++)
      {
        int next = INT32_MAX;
        int choose = -1;
        for (int leftEnd = best[L][R - 1]; leftEnd <= best[L + 1][R]; leftEnd++)
        {
          int cur = dp[L][leftEnd] + dp[leftEnd + 1][R];
          if (cur <= next)
          {
            next = cur;
            choose = leftEnd;
          }
        }
        best[L][R] = choose;
        dp[L][R] = next + w(s, L, R);
      }
    }
    return dp[0][N - 1];
  }

  static vector<int> randomArray(int len, int maxValue)
  {
    vector<int> arr(len);
    for (int i = 0; i < len; i++)
    {
      arr[i] = getRandom(0, maxValue);
    }
    return arr;
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
    int N = 15;
    int maxValue = 100;
    int testTime = 1000;
    cout << "测试开始" << endl;
    for (int i = 0; i < testTime; i++)
    {
      int len = getRandom(0, N);
      vector<int> arr = randomArray(len, maxValue);
      int ans1 = min1(arr);
      int ans2 = min2(arr);
      int ans3 = min3(arr);
      if (ans1 != ans2 || ans1 != ans3)
      {
        cout << "Oops!" << endl;
      }
    }
    cout << "测试结束" << endl;
  }
};

int main()
{
  StoneMerge::test();
  return 0;
}
