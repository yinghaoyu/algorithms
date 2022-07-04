#include <iostream>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

using TdArray = vector<vector<int>>;

class IsSum
{
 public:
  // arr中的值可能为正，可能为负，可能为0
  // 自由选择arr中的数字，能不能累加得到sum
  // 暴力递归方法
  static bool isSum1(vector<int> &arr, int sum)
  {
    if (sum == 0)
    {
      return true;
    }
    if (arr.size() == 0)
    {
      return false;
    }
    return process1(arr, arr.size() - 1, sum);
  }

  // 可以自由使用arr[0...i]上的数字，能不能累加得到sum
  static bool process1(vector<int> &arr, int i, int sum)
  {
    if (sum == 0)
    {
      return true;
    }
    if (i == -1)
    {
      return false;
    }
    return process1(arr, i - 1, sum) || process1(arr, i - 1, sum - arr[i]);
  }

  // arr中的值可能为正，可能为负，可能为0
  // 自由选择arr中的数字，能不能累加得到sum
  // 记忆化搜索方法
  // 从暴力递归方法来，加了记忆化缓存，就是动态规划了
  static bool isSum2(vector<int> &arr, int sum)
  {
    if (sum == 0)
    {
      return true;
    }
    if (arr.size() == 0)
    {
      return false;
    }
    unordered_map<int, unordered_map<int, bool>> path;
    return process2(arr, arr.size() - 1, sum, path);
  }

  static bool process2(vector<int> &arr, int i, int sum, unordered_map<int, unordered_map<int, bool>> &dp)
  {
    if (dp.find(i) != dp.end() && dp.at(i).find(sum) != dp.at(i).end())
    {
      return dp.at(i).at(sum);
    }
    bool ans = false;
    if (sum == 0)
    {
      ans = true;
    }
    else if (i != -1)
    {
      ans = process2(arr, i - 1, sum, dp) || process2(arr, i - 1, sum - arr[i], dp);
    }
    if (dp.find(i) == dp.end())
    {
      dp[i] = unordered_map<int, bool>{{sum, ans}};
    }
    return ans;
  }

  // arr中的值可能为正，可能为负，可能为0
  // 自由选择arr中的数字，能不能累加得到sum
  // 经典动态规划
  static bool isSum3(vector<int> &arr, int sum)
  {
    if (sum == 0)
    {
      return true;
    }
    if (arr.size() == 0)
    {
      return false;
    }
    int min = 0;  // 表示达到最小的值，大概率为负数
    int max = 0;  // 表示达到最大的值，大概率为正数
    for (int num : arr)
    {
      min += num < 0 ? num : 0;
      max += num > 0 ? num : 0;
    }
    if (sum < min || sum > max)
    {
      return false;
    }
    int N = arr.size();
    TdArray dp(N, vector<int>(max - min + 1));
    // 表示min这个值能取到，由于数组下标不能取负数
    // 所以这里-min表示取min的时候
    dp[0][-min] = true;
    dp[0][arr[0] - min] = true;  // arr[0]这个数纳入计算
    for (int i = 1; i < N; i++)
    {
      for (int j = min; j <= max; j++)  // 最小值 --> 最大值遍历
      {
        dp[i][j - min] = dp[i - 1][j - min];
        int next = j - min - arr[i];
        dp[i][j - min] |= (next >= 0 && next <= max - min && dp[i - 1][next]);
      }
    }
    return dp[N - 1][sum - min];
  }

  // arr中的值可能为正，可能为负，可能为0
  // 自由选择arr中的数字，能不能累加得到sum
  // 分治的方法
  // 如果arr中的数值特别大，动态规划方法依然会很慢
  // 此时如果arr的数字个数不算多(40以内)，哪怕其中的数值很大，分治的方法也将是最优解
  static bool isSum4(vector<int> &arr, int sum)
  {
    if (sum == 0)
    {
      return true;
    }
    if (arr.size() == 0)
    {
      return false;
    }
    if (arr.size() == 1)
    {
      return arr[0] == sum;
    }
    int N = arr.size();
    int mid = N >> 1;
    unordered_set<int> leftSum;
    unordered_set<int> rightSum;
    process4(arr, 0, mid, 0, leftSum);
    process4(arr, mid, N, 0, rightSum);
    for (int l : leftSum)
    {
      if (rightSum.find(sum - l) != rightSum.end())
      {
        return true;
      }
    }
    return false;
  }

  static void process4(vector<int> &arr, int i, int end, int pre, unordered_set<int> &ans)
  {
    if (i == end)
    {
      ans.emplace(pre);
    }
    else
    {
      process4(arr, i + 1, end, pre, ans);
      process4(arr, i + 1, end, pre + arr[i], ans);
    }
  }

  // 为了测试
  // 生成长度为len的随机数组
  // 值在[-max, max]上随机
  static vector<int> randomArray(int len, int max)
  {
    vector<int> arr(len);
    for (int i = 0; i < len; i++)
    {
      arr[i] = getRandom(1, max << 1) - max;
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

  // 对数器验证所有方法
  static void test()
  {
    int N = 20;
    int M = 100;
    int testTime = 100000;
    cout << "测试开始" << endl;
    for (int i = 0; i < testTime; i++)
    {
      int size = getRandom(1, N);
      vector<int> arr = randomArray(size, M);
      int sum = getRandom(1, M << 1) - M;
      bool ans1 = isSum1(arr, sum);
      bool ans2 = isSum2(arr, sum);
      bool ans3 = isSum3(arr, sum);
      bool ans4 = isSum4(arr, sum);
      if (ans1 ^ ans2 || ans3 ^ ans4 || ans1 ^ ans3)
      {
        cout << "出错了！" << endl;
        cout << "arr : ";
        for (int num : arr)
        {
          cout << num << " ";
        }
        cout << endl;
        cout << "sum : " << sum << endl;
        cout << "方法一答案 : " << ans1 << endl;
        cout << "方法二答案 : " << ans2 << endl;
        cout << "方法三答案 : " << ans3 << endl;
        cout << "方法四答案 : " << ans4 << endl;
        break;
      }
    }
    cout << "测试结束" << endl;
  }
};
int main()
{
  IsSum::test();
  return 0;
}
