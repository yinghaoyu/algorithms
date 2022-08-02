#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

// 比如{ 5, 3, 1, 4 }
// 全部数字对是：(5,3)、(5,1)、(5,4)、(3,1)、(3,4)、(1,4)
// 数字对的差值绝对值： 2、4、1、2、1、3
// 差值绝对值排序后：1、1、2、2、3、4
// 给定一个数组arr，和一个正数k
// 返回arr中所有数字对差值的绝对值，第k小的是多少
// arr = { 5, 3, 1, 4 }, k = 4
// 返回2
class MinKthPairMinusABS
{
 public:
  // 暴力解，生成所有数字对差值绝对值，排序，拿出第k个，k从1开始
  static int kthABS1(vector<int> &arr, int k)
  {
    int n = arr.size();
    int m = ((n - 1) * n) >> 1;  // 第2个元素开头的有n-1对，第2个元素开头的有n-2对，...
    if (m == 0 || k < 1 || k > m)
    {
      return -1;
    }
    vector<int> abs(m);
    int size = 0;
    for (int i = 0; i < n; i++)
    {
      for (int j = i + 1; j < n; j++)
      {
        abs[size++] = std::abs(arr[i] - arr[j]);
      }
    }
    std::sort(abs.begin(), abs.end());
    return abs[k - 1];
  }

  // 二分 + 双指针不回退
  // 时间复杂度O(Nlog(max - min))
  static int kthABS2(vector<int> &arr, int k)
  {
    int n = arr.size();
    if (n < 2 || k < 1 || k > ((n * (n - 1)) >> 1))
    {
      return -1;
    }
    std::sort(arr.begin(), arr.end());
    // 0 ~ 大-小 二分
    // l  ~  r
    int left = 0;
    int right = arr[n - 1] - arr[0];
    int mid = 0;
    int rightest = -1;
    while (left <= right)
    {
      mid = (left + right) / 2;
      // 数字对差值的绝对值<=mid的数字对个数，是不是 < k个的！
      if (valid(arr, mid, k))
      {
        rightest = mid;
        left = mid + 1;
      }
      else
      {
        right = mid - 1;
      }
    }
    return rightest + 1;
  }

  // 假设arr中的所有数字对，差值绝对值<=limit的个数为x
  // 如果 x < k，达标，返回true
  // 如果 x >= k，不达标，返回false
  static bool valid(vector<int> &arr, int limit, int k)
  {
    int x = 0;
    for (int l = 0, r = 1; l < arr.size(); r = std::max(r, ++l))
    {
      while (r < arr.size() && arr[r] - arr[l] <= limit)
      {
        r++;
      }
      x += r - l - 1;
    }
    return x < k;
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
  static vector<int> randomArray(int n, int v)
  {
    vector<int> ans(n);
    for (int i = 0; i < ans.size(); i++)
    {
      ans[i] = getRandom(0, v);
    }
    return ans;
  }

  // 为了测试
  static void test()
  {
    int size = 100;
    int value = 100;
    int testTime = 10000;
    cout << "测试开始" << endl;
    for (int i = 0; i < testTime; i++)
    {
      int n = getRandom(0, size);
      int k = getRandom(1, n * (n - 1) / 2);
      vector<int> arr = randomArray(n, value);
      int ans1 = kthABS1(arr, k);
      int ans2 = kthABS2(arr, k);
      if (ans1 != ans2)
      {
        cout << "arr : ";
        for (int num : arr)
        {
          cout << num << " ";
        }
        cout << endl;
        cout << "k : " << k << endl;
        cout << "ans1 : " << ans1 << endl;
        cout << "ans2 : " << ans2 << endl;
        cout << "出错了！" << endl;
        break;
      }
    }
    cout << "测试结束" << endl;

    int n = 500000;
    int v = 50000000;
    vector<int> arr = randomArray(n, v);
    int k = getRandom(1, n * (n - 1) / 2);
    clock_t start = clock();
    kthABS2(arr, k);
    clock_t end = clock();
    cout << "大数据量运行时间（毫秒）：" << static_cast<double>(end - start) / CLOCKS_PER_SEC << "ms" << endl;
  }
};

int main()
{
  MinKthPairMinusABS::test();
  return 0;
}
