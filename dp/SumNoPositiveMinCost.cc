#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

using namespace std;
using TdArrdy = vector<vector<int>>;

// 给定一个正数数组arr长度为n、正数x、正数y
// 你的目标是让arr整体的累加和<=0
// 你可以对数组中的数num执行以下三种操作中的一种，且每个数最多能执行一次操作 :
// 1）不变
// 2）可以选择让num变成0，承担x的代价
// 3）可以选择让num变成-num，承担y的代价
// 返回你达到目标的最小代价
// 数据规模 : 面试时面试官没有说数据规模
class SumNoPositiveMinCost
{
 public:
  // 动态规划
  static int minOpStep1(vector<int> &arr, int x, int y)
  {
    int sum = 0;
    for (int num : arr)
    {
      sum += num;
    }
    return process1(arr, x, y, 0, sum);
  }

  // arr[i...]自由选择，每个位置的数可以执行三种操作中的一种！
  // 执行变0的操作，x操作，代价 -> x
  // 执行变相反数的操作，y操作，代价 -> y
  // 还剩下sum这么多累加和，需要去搞定！
  // 返回搞定了sum，最低代价是多少？
  static int process1(vector<int> &arr, int x, int y, int i, int sum)
  {
    if (sum <= 0)
    {
      return 0;
    }
    // sum > 0 没搞定
    if (i == arr.size())
    {
      return INT32_MAX;
    }
    // 第一选择，什么也不干！
    int p1 = process1(arr, x, y, i + 1, sum);
    // 第二选择，执行x的操作，变0 x + 后续
    int p2 = INT32_MAX;
    int next2 = process1(arr, x, y, i + 1, sum - arr[i]);
    if (next2 != INT32_MAX)
    {
      p2 = x + next2;
    }
    // 第三选择，执行y的操作，变相反数 x + 后续 7 -7 -14
    int p3 = INT32_MAX;
    int next3 = process1(arr, x, y, i + 1, sum - (arr[i] << 1));
    if (next3 != INT32_MAX)
    {
      p3 = y + next3;
    }
    return std::min(p1, std::min(p2, p3));
  }

  // 贪心（最优解）,时间复杂度O(N*logN)
  static int minOpStep2(vector<int> &arr, int x, int y)
  {
    std::sort(arr.begin(), arr.end());  // 小 -> 大
    int n = arr.size();
    for (int l = 0, r = n - 1; l <= r; l++, r--)
    {
      int tmp = arr[l];
      arr[l] = arr[r];
      arr[r] = tmp;
    }
    // arr 大 -> 小
    if (x >= y)
    {
      // 没有任何必要执行x操作
      // 因为x操作让值变0，y操作让值变负数，变零的代价更大，根据贪心，明显不执行x操作
      int sum = 0;
      for (int num : arr)
      {
        sum += num;
      }
      int cost = 0;
      for (int i = 0; i < n && sum > 0; i++)
      {
        sum -= arr[i] << 1;
        cost += y;
      }
      return cost;
    }
    else
    {
      // x < y
      // 例如 arr = {100 90 ... 7 3}
      // 如果前面的数执行y操作停下来了，那么后面的数肯定不执行y操作，因为后面执行y操作所获得的负数值更小
      // 同理，如果前面的数执行x操作停下来了，那么后面的数肯定不执行x操作，因为后面执行x操作所获得的差值更小
      for (int i = n - 2; i >= 0; i--)
      {
        arr[i] += arr[i + 1];  // 后缀和
      }
      int benefit = 0;
      // 注意，可以不二分，用不回退的方式！
      // 执行Y操作的数，有0个的时候
      int left = mostLeft(arr, 0, benefit);
      int cost = left * x;
      for (int i = 0; i < n - 1; i++)
      {
        // 0..i 这些数，都执行Y
        benefit += arr[i] - arr[i + 1];
        // 从后缀数组中找出最左边第一个 <= benefit的数的位置
        left = mostLeft(arr, i + 1, benefit);
        cost = std::min(cost, (i + 1) * y + (left - i - 1) * x);
      }
      return cost;
    }
  }

  // arr是后缀和数组， arr[l...]中找到值<=v的最左位置
  static int mostLeft(vector<int> &arr, int l, int v)
  {
    int r = arr.size() - 1;
    int m = 0;
    int ans = arr.size();
    while (l <= r)
    {
      m = (l + r) / 2;
      if (arr[m] <= v)
      {
        ans = m;
        r = m - 1;
      }
      else
      {
        l = m + 1;
      }
    }
    return ans;
  }

  // 不回退
  static int minOpStep3(vector<int> &arr, int x, int y)
  {
    // 系统排序，小 -> 大
    std::sort(arr.begin(), arr.end());
    int n = arr.size();
    // 如何变成 大 -> 小
    for (int l = 0, r = n - 1; l <= r; l++, r--)
    {
      int tmp = arr[l];
      arr[l] = arr[r];
      arr[r] = tmp;
    }
    if (x >= y)
    {
      int sum = 0;
      for (int num : arr)
      {
        sum += num;
      }
      int cost = 0;
      for (int i = 0; i < n && sum > 0; i++)
      {
        sum -= arr[i] << 1;
        cost += y;
      }
      return cost;
    }
    else
    {
      // 0个数执行Y
      int benefit = 0;
      // 全部的数都需要执行x，才能让累加和<=0
      int cost = arr.size() * x;
      int holdSum = 0;
      for (int yRight = 0, holdLeft = n; yRight < holdLeft - 1; yRight++)
      {
        benefit += arr[yRight];
        while (holdLeft - 1 > yRight && holdSum + arr[holdLeft - 1] <= benefit)
        {
          holdSum += arr[holdLeft - 1];
          holdLeft--;
        }
        // 0...yRight x holdLeft....
        cost = std::min(cost, (yRight + 1) * y + (holdLeft - yRight - 1) * x);
      }
      return cost;
    }
  }

  // 为了测试
  static vector<int> randomArray(int len, int v)
  {
    vector<int> arr(len);
    for (int i = 0; i < len; i++)
    {
      arr[i] = getRandom(1, v);
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

  // 为了测试
  static void test()
  {
    int n = 12;
    int v = 20;
    int c = 10;
    int testTime = 10000;
    cout << "测试开始" << endl;
    for (int i = 0; i < testTime; i++)
    {
      int len = getRandom(0, n);
      vector<int> arr = randomArray(len, v);
      vector<int> arr1 = arr;
      vector<int> arr2 = arr;
      vector<int> arr3 = arr;
      int x = getRandom(0, c);
      int y = getRandom(0, c);
      int ans1 = minOpStep1(arr1, x, y);
      int ans2 = minOpStep2(arr2, x, y);
      int ans3 = minOpStep3(arr3, x, y);
      if (ans1 != ans2 || ans1 != ans3)
      {
        cout << "出错了!" << endl;
      }
    }
    cout << "测试结束" << endl;
  }
};

int main()
{
  return 0;
}
