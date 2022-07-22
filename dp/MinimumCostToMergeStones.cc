#include <iostream>
#include <random>
#include <vector>

using namespace std;
using TtArray = vector<vector<vector<int>>>;

// dp/StoneMerge.cc的扩展版

// 测试链接 : https://leetcode.com/problems/minimum-cost-to-merge-stones/

class MinimumCostToMergeStones
{
  //	// arr[L...R]一定要整出P份，合并的最小代价，返回！
  //	public static int f(int[] arr, int K, int L, int R, int P) {
  //		if(从L到R根本不可能弄出P份) {
  //			return Integer.MAX_VALUE;
  //		}
  //		// 真的有可能的！
  //		if(P == 1) {
  //			return L == R ? 0 : (f(arr, K, L, R, K) + 最后一次大合并的代价);
  //		}
  //		int ans = Integer.MAX_VALUE;
  //		// 真的有可能，P > 1
  //		for(int i = L; i < R;i++) {
  //			// L..i(1份)  i+1...R(P-1份)
  //			int left = f(arr, K, L, i, 1);
  //			if(left == Integer.MAX_VALUE) {
  //				continue;
  //			}
  //			int right = f(arr, K, i+1,R,P-1);
  //			if(right == Integer.MAX_VALUE) {
  //				continue;
  //			}
  //			int all = left + right;
  //			ans = Math.min(ans, all);
  //		}
  //		return ans;
  //	}

 public:
  static int mergeStones1(vector<int> &stones, int K)
  {
    int n = stones.size();
    if ((n - 1) % (K - 1) > 0)  // n个数，每次K个数合并，最终能不能变为1个数，观察得到的关系式
    {
      return -1;
    }
    vector<int> presum(n + 1);
    for (int i = 0; i < n; i++)
    {
      presum[i + 1] = presum[i] + stones[i];
    }
    return process1(0, n - 1, 1, stones, K, presum);
  }

  // part >= 1
  // arr[L..R] 一定要弄出P份，返回最低代价
  // arr、K、presum（前缀累加和数组，求i..j的累加和，就是O(1)了）
  static int process1(int L, int R, int P, vector<int> &arr, int K, vector<int> &presum)
  {
    if (L == R)
    {
      // arr[L..R]
      // 只剩1个数，弄出P份，无需代价
      return P == 1 ? 0 : -1;
    }
    // L ... R 不只一个数
    if (P == 1)
    {
      // 把L...R合并成K份，这K份最终才能合并成1份
      int next = process1(L, R, K, arr, K, presum);
      if (next == -1)
      {
        return -1;
      }
      else
      {
        // 把L...R合并成K份的代价 + K份合并成1份的代价
        return next + presum[R + 1] - presum[L];
      }
    }
    else
    {  // P > 1
      int ans = INT32_MAX;
      // L...mid是第1块，剩下的是part-1块
      // mid 每次+ (K - 1)是为了过滤无效项
      // 例如：
      // K = 3
      // arr = {1, 2, 3, 4, 5, 6}
      //           ↑
      //          mid            --> 这个时候1、2只有两个数，不能合并
      for (int mid = L; mid < R; mid += K - 1)
      {
        // L..mid(一份) mid+1...R(part - 1)
        int next1 = process1(L, mid, 1, arr, K, presum);
        int next2 = process1(mid + 1, R, P - 1, arr, K, presum);
        if (next1 != -1 && next2 != -1)
        {
          // 尝试各种结果，取最小值返回
          ans = std::min(ans, next1 + next2);
        }
      }
      return ans;
    }
  }

  static int mergeStones2(vector<int> &stones, int K)
  {
    int n = stones.size();
    if ((n - 1) % (K - 1) > 0)
    {
      // n个数，到底能不能K个相邻的数合并，最终变成1个数！
      return -1;
    }
    vector<int> presum(n + 1);
    for (int i = 0; i < n; i++)
    {
      presum[i + 1] = presum[i] + stones[i];
    }
    TtArray dp(n, vector<vector<int>>(n, vector<int>(K + 1)));
    return process2(0, n - 1, 1, stones, K, presum, dp);
  }

  static int process2(int L, int R, int P, vector<int> &arr, int K, vector<int> &presum, TtArray &dp)
  {
    if (dp[L][R][P] != 0)
    {
      return dp[L][R][P];
    }
    if (L == R)
    {
      return P == 1 ? 0 : -1;
    }
    if (P == 1)
    {
      int next = process2(L, R, K, arr, K, presum, dp);
      if (next == -1)
      {
        dp[L][R][P] = -1;
        return -1;
      }
      else
      {
        dp[L][R][P] = next + presum[R + 1] - presum[L];
        return next + presum[R + 1] - presum[L];
      }
    }
    else
    {
      int ans = INT32_MAX;
      // i...mid是第1块，剩下的是part-1块
      for (int mid = L; mid < R; mid += K - 1)
      {
        int next1 = process2(L, mid, 1, arr, K, presum, dp);
        int next2 = process2(mid + 1, R, P - 1, arr, K, presum, dp);
        if (next1 == -1 || next2 == -1)
        {
          dp[L][R][P] = -1;
          return -1;
        }
        else
        {
          ans = std::min(ans, next1 + next2);
        }
      }
      dp[L][R][P] = ans;
      return ans;
    }
  }

  static int getRandom(int min, int max)
  {
    random_device seed;                            // 硬件生成随机数种子
    ranlux48 engine(seed());                       // 利用种子生成随机数引
    uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
    int res = distrib(engine);                     // 随机数
    return res;
  }
  // for test
  static vector<int> generateRandomArray(int maxSize, int maxValue)
  {
    vector<int> arr(getRandom(1, maxSize));
    for (int i = 0; i < arr.size(); i++)
    {
      arr[i] = getRandom(0, maxValue);
    }
    return arr;
  }

  // for test
  static void printArray(vector<int> &arr)
  {
    if (arr.size() == 0)
    {
      return;
    }
    for (int i = 0; i < arr.size(); i++)
    {
      cout << arr[i] << " ";
    }
    cout << endl;
  }

  static void test()
  {
    int maxSize = 12;
    int maxValue = 100;
    cout << "Test begin" << endl;
    for (int testTime = 0; testTime < 100000; testTime++)
    {
      vector<int> arr = generateRandomArray(maxSize, maxValue);
      int K = getRandom(1, 7) + 2;
      int ans1 = mergeStones1(arr, K);
      int ans2 = mergeStones2(arr, K);
      if (ans1 != ans2)
      {
        cout << ans1 << endl;
        cout << ans2 << endl;
      }
    }
  }
};

int main()
{
  MinimumCostToMergeStones::test();
  return 0;
}
