#include <iostream>
#include <random>
#include <string>
#include <vector>

using namespace std;

// 给定一个只由小写字母组成的字符串str，长度为N
// 给定一个只由0、1组成的数组arr，长度为N
// arr[i] == 0表示str中i位置的字符不许修改
// arr[i] == 1表示str中i位置的字符允许修改
// 给定一个正数m，表示在任意允许修改的位置，可以把该位置的字符变成a~z中的任何一个
// 可以修改m次
// 返回在最多修改m次的情况下，全是一种字符的最长子串是多长
// 1 <= N, M <= 10^5
// 所有字符都是小写
class MaxLengthSameCharMChanges
{
 public:
  // 暴力方法
  // 为了测试
  static int maxLen1(string str, vector<int> arr, int m)
  {
    int n = str.length();
    int ans = 0;
    // 把字符全部修改成a b c ... z
    for (char c = 'a'; c <= 'z'; c++)
    {
      // 枚举所有区间
      for (int i = 0; i < n; i++)
      {
        for (int j = n - 1; j >= i; j--)
        {
          // s[i...j]之间的字符全部修改成c，最多修改m次能否成功
          if (ok(str, i, j, c, arr, m))
          {
            ans = std::max(ans, j - i + 1);
            break;
          }
        }
      }
    }
    return ans;
  }

  // 为了测试
  // s[l...r]之间的字符全部修改成c，最多修改m次能否成功
  static bool ok(string s, int l, int r, char c, vector<int> arr, int m)
  {
    for (int i = l; i <= r; i++)
    {
      if (s[i] == c)
      {
        // 已经达标不需要修改
        continue;
      }
      if (arr[i] == 0 || m == 0)
      {
        // 存在一个字符不可修改，或者超过了修改次数
        return false;
      }
      m--;
    }
    return true;
  }

  // 正式方法
  // 时间复杂度O(N)
  static int maxLen2(string str, vector<int> arr, int m)
  {
    int n = str.length();
    int ans = 0;
    for (char c = 'a'; c <= 'z'; c++)
    {
      int r = 0;
      int change = 0;
      for (int l = 0; l < n; l++)
      {
        // 滑动窗口
        // [l..r)
        // [l...r-1] r
        while (r < n)
        {
          if (str[r] == c)
          {
            // 已经达标不需要修改
            r++;
            continue;
          }
          // s[r] != 你的要求
          if (arr[r] == 0 || change == m)
          {
            break;
          }
          // arr[r] == 1 &&  change < m
          change++;
          r++;
        }
        // [l...r-1] r
        ans = std::max(ans, r - l);
        // [l....r-1] [l]吐出来！
        if (str[l] != c && arr[l] == 1)
        {
          // 因为l马上要+1了，l如果之前满足要求，那么需要--
          change--;
        }
        r = std::max(r, l + 1);  // 右边界一定在左边界的右边
      }
    }
    return ans;
  }

  // 为了测试
  static string randomString(int n, int r)
  {
    string ans(n, ' ');
    for (int i = 0; i < n; i++)
    {
      ans[i] = getRandom(0, r) + 'a';
    }
    return ans;
  }

  // 为了测试
  static vector<int> randomArray(int n)
  {
    vector<int> ans(n);
    for (int i = 0; i < n; i++)
    {
      ans[i] = getRandom(0, 1);
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

  // 为了测试
  static void test()
  {
    int N = 100;
    int R = 5;
    int testTimes = 5000;
    cout << "测试开始" << endl;
    for (int i = 0; i < testTimes; i++)
    {
      int n = getRandom(1, N);
      int m = getRandom(1, n);
      string str = randomString(n, R);
      vector<int> arr = randomArray(n);
      int ans1 = maxLen1(str, arr, m);
      int ans2 = maxLen2(str, arr, m);
      if (ans1 != ans2)
      {
        cout << "出错了!" << endl;
        break;
      }
    }
    cout << "测试结束" << endl;
  }
};

int main()
{
  MaxLengthSameCharMChanges::test();
  return 0;
}
