#include <iostream>
#include <map>
#include <vector>

using namespace std;
using TdArray = vector<vector<int>>;

// 题目描述：
// 背包容量为w，第i袋零食体积为arr[i]，不超过背包容量下，一共有多少种放法？
// 每件零食可选择放或者不放，总体积为0也算一种放法
//
// 测试链接：https://www.nowcoder.com/questionTerminal/d94bb2fa461d42bcb4c0f2b94f5d4281

class SnacksWays
{
 public:
  // 暴力递归
  static int ways1(vector<int> &arr, int w)
  {
    // arr[0...]
    return process(arr, 0, w);
  }

  // 从左往右的经典模型
  // 还剩的容量是rest，arr[index...]自由选择，
  // 返回选择方案
  // index ： 0～N
  // rest : 0~w
  static int process(vector<int> &arr, int index, int rest)
  {
    if (rest < 0)
    {
      // 没有容量了
      // -1 无方案的意思
      return -1;
    }
    // rest>=0,
    if (index == arr.size())
    {
      // 无零食可选
      return 1;
    }
    // rest >=0
    // 有零食index
    // index号零食，要 or 不要
    // index, rest
    // (index+1, rest)
    // (index+1, rest-arr[i])
    int next1 = process(arr, index + 1, rest);               // 不要
    int next2 = process(arr, index + 1, rest - arr[index]);  // 要
    return next1 + (next2 == -1 ? 0 : next2);
  }

  // 暴力递归改dp
  static int ways2(vector<int> &arr, int w)
  {
    int N = arr.size();
    TdArray dp(N + 1, vector<int>(w + 1));
    for (int j = 0; j <= w; j++)
    {
      // 体积和没超过背包容量w都算1种方案
      dp[N][j] = 1;
    }
    for (int i = N - 1; i >= 0; i--)
    {
      for (int j = 0; j <= w; j++)
      {
        dp[i][j] = dp[i + 1][j] + ((j - arr[i] >= 0) ? dp[i + 1][j - arr[i]] : 0);
      }
    }
    return dp[0][w];
  }

  // 四边形不等式 ?
  static int ways3(vector<int> &arr, int w)
  {
    int N = arr.size();
    TdArray dp(N, vector<int>(w + 1));
    for (int i = 0; i < N; i++)
    {
      dp[i][0] = 1;
    }
    if (arr[0] <= w)
    {
      dp[0][arr[0]] = 1;
    }
    for (int i = 1; i < N; i++)
    {
      for (int j = 1; j <= w; j++)
      {
        dp[i][j] = dp[i - 1][j] + ((j - arr[i]) >= 0 ? dp[i - 1][j - arr[i]] : 0);
      }
    }
    int ans = 0;
    for (int j = 0; j <= w; j++)
    {
      ans += dp[N - 1][j];
    }
    return ans;
  }

  // 分治法
  static int ways4(vector<int> &arr, int bag)
  {
    if (arr.size() == 0)
    {
      return 0;
    }
    if (arr.size() == 1)
    {
      return arr[0] <= bag ? 2 : 1;
    }
    // 分治法
    int mid = (arr.size() - 1) >> 1;
    map<int, int> lmap;
    // 在0 ~ mid之间的方法数
    int ways = process4(arr, 0, mid, 0, bag, lmap);
    map<int, int> rmap;
    // 在mid+1 ~ arr.size()-1之间的方法数
    ways += process4(arr, mid + 1, arr.size() - 1, 0, bag, rmap);
    // 生成右边前缀和map
    map<int, int> rpre;
    int pre = 0;
    for (auto entry : rmap)
    {
      pre += entry.second;
      rpre[entry.first] = pre;
    }
    // 左边的map去匹配
    for (auto entry : lmap)
    {
      int lweight = entry.first;
      int lways = entry.second;
      // 从右边到小于等于该数且离他最近的那个数
      // upper_bound 找到第一个>的数
      auto floor = rpre.upper_bound(bag - lweight);
      // floor--后 此时floor 是 <= 最近的数，相当于floorKey
      floor--;
      // 为什么要计算前缀和？
      // 右侧只要零食大小不超过floor，都视为符合条件，找出他们的方法数
      if (floor != rpre.end())
      {
        int rways = rpre.at(floor->first);
        ways += lways * rways;  // 左右两边组合的方法数
      }
    }
    // 为什么要加1，左边全不选，右边也全不选
    return ways + 1;
  }

  // arr 30
  // process4(arr, 0, 14, 0, bag, map)

  // process4(arr, 15, 29, 0, bag, map)

  // 从index出发，到end结束
  // 之前的选择，已经形成的累加和sum
  // 零食[index....end]自由选择，出来的所有累加和，不能超过bag，每一种累加和对应的方法数，填在map里
  // 最后不能什么货都没选
  static int process4(vector<int> &arr, int index, int end, int sum, int bag, map<int, int> &map)
  {
    if (sum > bag)
    {
      return 0;
    }
    // sum <= bag
    if (index > end)
    {
      // 所有商品自由选择完了！
      if (sum != 0)
      {
        if (map.find(sum) == map.end())
        {
          map[sum] = 1;
        }
        else
        {
          map[sum]++;
        }
        return 1;
      }
      else
      {
        return 0;
      }
    }
    // sum <= bag 并且 index <= end(还有货)
    // 1) 不要当前index位置的货
    int ways = process4(arr, index + 1, end, sum, bag, map);

    // 2) 要当前index位置的货
    ways += process4(arr, index + 1, end, sum + arr[index], bag, map);
    return ways;
  }

  static void test()
  {
    vector<int> arr{1, 2, 4};
    int w = 10;
    cout << ways1(arr, w) << endl;
    cout << ways2(arr, w) << endl;
    cout << ways3(arr, w) << endl;
    cout << ways4(arr, w) << endl;
  }
};

int main()
{
  SnacksWays::test();
  return 0;
}
