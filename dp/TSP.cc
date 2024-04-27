#include <cstdint>
#include <iostream>
#include <random>
#include <unordered_map>
#include <vector>

using namespace std;
using TdArray = vector<vector<int>>;

// 问题描述：
// 旅行商问题，即TSP问题（Traveling Salesman Problem）又译为旅行推销员问题、货郎担问题，是数学领域中著名问题之一。
//
// 假设有一个旅行商人要拜访n个城市，
// 他必须选择所要走的路径，
// 路径的限制是每个城市只能拜访一次，
// 而且最后要回到原来出发的城市。
// 路径的选择目标是要求得的路径路程为所有路径之中的最小值。

// 状压dp
// https://www.bilibili.com/video/BV15a4y1o7NA/
class TSP
{
 public:
  static int t1(TdArray& matrix)
  {
    int N = matrix.size();  // 0...N-1
    // set
    // set.at(i) == 1 i这座城市在集合里
    // set.at(i) == -1 i这座城市不在集合里
    unordered_map<int, int> set;
    for (int i = 0; i < N; i++)
    {
      set[i] = 1;
    }
    return func1(matrix, set, 0);
  }

  // 任何两座城市之间的距离，可以在matrix里面拿到
  // set中表示着哪些城市的集合，
  // start这座城一定在set里，
  // 从start出发，要把set中所有的城市过一遍，最终回到0这座城市，最小距离是多少
  static int func1(TdArray& matrix, unordered_map<int, int>& set, int start)
  {
    int cityNum = 0;
    for (int i = 0; i < set.size(); i++)
    {
      if (set.at(i) != -1)
      {
        cityNum++;  // 统计集合的城市数量
      }
    }
    if (cityNum == 1)
    {
      return matrix[start][0];
    }
    // cityNum > 1  不只start这一座城
    set[start] = -1;
    int min = INT32_MAX;
    for (int i = 0; i < set.size(); i++)
    {
      if (set.at(i) != -1)
      {
        // start -> i i... -> 0
        int cur = matrix[start][i] + func1(matrix, set, i);
        min = std::min(min, cur);
      }
    }
    set[start] = 1;
    return min;
  }

  static int t2(TdArray& matrix)
  {
    int N = matrix.size();  // 0...N-1
    // 7座城 1111111
    int allCity = (1 << N) - 1;
    return f2(matrix, allCity, 0);
  }

  // 任何两座城市之间的距离，可以在matrix里面拿到
  // set中表示着哪些城市的集合，
  // start这座城一定在set里，
  // 从start出发，要把set中所有的城市过一遍，最终回到0这座城市，最小距离是多少
  static int f2(TdArray& matrix, int cityStatus, int start)
  {
    if (cityStatus == (cityStatus & (~cityStatus + 1)))  // lowBit
    {
      return matrix[start][0];
    }

    // 把start位的1去掉，
    cityStatus &= (~(1 << start));
    int min = INT32_MAX;
    // 枚举所有的城市
    for (int move = 0; move < matrix.size(); move++)
    {
      if ((cityStatus & (1 << move)) != 0)
      {
        int cur = matrix[start][move] + f2(matrix, cityStatus, move);
        min = std::min(min, cur);
      }
    }
    cityStatus |= (1 << start);
    return min;
  }

  static int t3(TdArray& matrix)
  {
    int N = matrix.size();  // 0...N-1
    // 7座城 1111111
    int allCity = (1 << N) - 1;
    TdArray dp(1 << N, vector<int>(N));
    for (int i = 0; i < (1 << N); i++)
    {
      for (int j = 0; j < N; j++)
      {
        dp[i][j] = -1;
      }
    }
    return f3(matrix, allCity, 0, dp);
  }

  // 任何两座城市之间的距离，可以在matrix里面拿到
  // set中表示着哪些城市的集合，
  // start这座城一定在set里，
  // 从start出发，要把set中所有的城市过一遍，最终回到0这座城市，最小距离是多少
  static int f3(TdArray& matrix, int cityStatus, int start, TdArray& dp)
  {
    if (dp[cityStatus][start] != -1)
    {
      return dp[cityStatus][start];
    }
    if (cityStatus == (cityStatus & (~cityStatus + 1)))
    {
      dp[cityStatus][start] = matrix[start][0];
    }
    else
    {
      // 把start位的1去掉，
      cityStatus &= (~(1 << start));
      int min = INT32_MAX;
      // 枚举所有的城市
      for (int move = 0; move < matrix.size(); move++)
      {
        if (move != start && (cityStatus & (1 << move)) != 0)
        {
          int cur = matrix[start][move] + f3(matrix, cityStatus, move, dp);
          min = std::min(min, cur);
        }
      }
      cityStatus |= (1 << start);
      dp[cityStatus][start] = min;
    }
    return dp[cityStatus][start];
  }

  static int t4(TdArray& matrix)
  {
    int N = matrix.size();  // 0...N-1
    int statusNums = 1 << N;
    TdArray dp(statusNums, vector<int>(N));

    for (int status = 0; status < statusNums; status++)
    {
      for (int start = 0; start < N; start++)
      {
        if ((status & (1 << start)) != 0)
        {
          if (status == (status & (~status + 1)))
          {
            dp[status][start] = matrix[start][0];
          }
          else
          {
            int min = INT32_MAX;
            // start 城市在status里去掉之后，的状态
            int preStatus = status & (~(1 << start));
            // start -> i
            for (int i = 0; i < N; i++)
            {
              if ((preStatus & (1 << i)) != 0)
              {
                int cur = matrix[start][i] + dp[preStatus][i];
                min = std::min(min, cur);
              }
            }
            dp[status][start] = min;
          }
        }
      }
    }
    return dp[statusNums - 1][0];
  }

  // matrix[i][j] -> i城市到j城市的距离
  static int tsp1(TdArray& matrix, int origin)
  {
    if (matrix.size() < 2 || origin < 0 || origin >= matrix.size())
    {
      return 0;
    }
    // 要考虑的集合
    unordered_map<int, int> cities;
    // cities[0] == 1 表示0城在集合里
    // cities[i] == 1 表示i城在集合里
    for (int i = 0; i < matrix.size(); i++)
    {
      cities[i] = 1;
    }
    // null,1,1,1,1,1,1
    // origin城不参与集合
    cities[origin] = -1;
    return process(matrix, origin, cities, origin);
  }

  // matrix 所有距离，存在其中
  // origin 固定参数，唯一的目标
  // cities 要考虑的集合，一定不含有origin
  // 当前来到的城市是谁，cur
  static int process(TdArray& matrix, int aim, unordered_map<int, int>& cities, int cur)
  {
    bool hasCity = false;  // 集团中还是否有城市
    int ans = INT32_MAX;
    for (int i = 0; i < cities.size(); i++)
    {
      if (cities.at(i) != -1)
      {
        hasCity = true;
        cities[i] = -1;
        // matrix[cur][i] + f(i, 集团(去掉i) )
        ans = std::min(ans, matrix[cur][i] + process(matrix, aim, cities, i));
        cities[i] = 1;
      }
    }
    return hasCity ? ans : matrix[cur][aim];
  }

  // cities 里，一定含有cur这座城
  // 解决的是，集合从cur出发，通过集合里所有的城市，最终来到aim，最短距离
  static int process2(TdArray& matrix, int aim, unordered_map<int, int>& cities, int cur)
  {
    if (cities.size() == 1)
    {
      return matrix[cur][aim];
    }
    cities[cur] = -1;
    int ans = INT32_MAX;
    for (int i = 0; i < cities.size(); i++)
    {
      if (cities.at(i) != -1)
      {
        int dis = matrix[cur][i] + process2(matrix, aim, cities, i);
        ans = std::min(ans, dis);
      }
    }
    cities[cur] = 1;
    return ans;
  }

  static int tsp2(TdArray& matrix, int origin)
  {
    if (matrix.size() < 2 || origin < 0 || origin >= matrix.size())
    {
      return 0;
    }
    int N = matrix.size() - 1;  // 除去origin之后是n-1个点
    int S = 1 << N;             // 状态数量
    TdArray dp(S, vector<int>(N));
    int icity = 0;
    int kcity = 0;
    for (int i = 0; i < N; i++)
    {
      icity = i < origin ? i : i + 1;
      // 00000000 i
      dp[0][i] = matrix[icity][origin];
    }
    for (int status = 1; status < S; status++)
    {
      // 尝试每一种状态 status = 0 0 1 0 0 0 0 0 0
      // 下标 8 7 6 5 4 3 2 1 0
      for (int i = 0; i < N; i++)
      {
        // i 枚举的出发城市
        dp[status][i] = INT32_MAX;
        if (((1 << i) & status) != 0)
        {
          // 如果i这座城是可以枚举的，i = 6 ， i对应的原始城的编号，icity
          icity = i < origin ? i : i + 1;
          for (int k = 0; k < N; k++)
          {
            // i 这一步连到的点，k
            if (((1 << k) & status) != 0)
            {
              // i 这一步可以连到k
              kcity = k < origin ? k : k + 1;  // k对应的原始城的编号，kcity
              dp[status][i] = std::min(dp[status][i], dp[status ^ (1 << i)][k] + matrix[icity][kcity]);
            }
          }
        }
      }
    }
    int ans = INT32_MAX;
    for (int i = 0; i < N; i++)
    {
      icity = i < origin ? i : i + 1;
      ans = std::min(ans, dp[S - 1][i] + matrix[origin][icity]);
    }
    return ans;
  }

  static TdArray generateGraph(int maxSize, int maxValue)
  {
    int len = getRandom(1, maxSize);
    TdArray matrix(len, vector<int>(len));
    for (int i = 0; i < len; i++)
    {
      for (int j = 0; j < len; j++)
      {
        matrix[i][j] = getRandom(1, maxValue);
      }
    }
    for (int i = 0; i < len; i++)
    {
      matrix[i][i] = 0;
    }
    return matrix;
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
    int len = 10;
    int value = 100;
    cout << "功能测试开始" << endl;
    for (int i = 0; i < 20000; i++)
    {
      TdArray matrix = generateGraph(len, value);
      int origin = getRandom(0, matrix.size() - 1);
      // int ans1 = t1(matrix);
      // int ans2 = t2(matrix);
      int ans3 = t3(matrix);
      int ans4 = t4(matrix);
      // int ans5 = tsp1(matrix, origin);
      int ans6 = tsp2(matrix, origin);
      // if (ans1 != ans2 || ans2 != ans3)
      //{
      // cout << "fuck" << endl;
      //}
      if (ans3 != ans4 || ans4 != ans6)
      {
        cout << "fuck" << endl;
      }
    }
    cout << "功能测试结束" << endl;

    len = 22;
    cout << "性能测试开始，数据规模 : " << len << endl;
    TdArray matrix(len, vector<int>(len));
    for (int i = 0; i < len; i++)
    {
      for (int j = 0; j < len; j++)
      {
        matrix[i][j] = getRandom(1, value);
      }
    }
    for (int i = 0; i < len; i++)
    {
      matrix[i][i] = 0;
    }
    clock_t start;
    clock_t end;
    double elapsedTime;
    start = clock();
    t4(matrix);
    end = clock();
    elapsedTime = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    cout << "运行时间 : " << elapsedTime << " 毫秒" << endl;
    cout << "性能测试结束" << endl;
  }
};

int main()
{
  TSP::test();
  return 0;
}
