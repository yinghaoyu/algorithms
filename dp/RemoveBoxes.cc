#include <iostream>
#include <vector>

using namespace std;

using TtArray = vector<vector<vector<int>>>;

// 本题测试链接 : https://leetcode.com/problems/remove-boxes/
class RemoveBoxes
{
 public:
  // arr[L...R]消除，而且前面跟着K个arr[L]这个数
  // 返回：所有东西都消掉，最大得分
  static int func1(vector<int> &arr, int L, int R, int K)
  {
    if (L > R)
    {
      return 0;
    }
    // 方案一：
    // 前面K个arr[L]，加上当前的arr[L]，一共有K+1个数
    int ans = func1(arr, L + 1, R, 0) + (K + 1) * (K + 1);

    // 前面的K个X，和arr[L]数，合在一起了，现在有K+1个arr[L]位置的数
    for (int i = L + 1; i <= R; i++)
    {
      if (arr[i] == arr[L])
      {
        // 方案二：
        // 此时先把L+1...i-1的先处理掉，这样i号元素就可以并到前面的arr[L]里计算
        ans = std::max(ans, func1(arr, L + 1, i - 1, 0) + func1(arr, i, R, K + 1));
      }
    }
    return ans;
  }

  static int removeBoxes1(vector<int> &boxes)
  {
    int N = boxes.size();
    TtArray dp(N, vector<vector<int>>(N, vector<int>(N)));
    int ans = process1(boxes, 0, N - 1, 0, dp);
    return ans;
  }

  static int process1(vector<int> &boxes, int L, int R, int K, TtArray dp)
  {
    if (L > R)
    {
      return 0;
    }
    if (dp[L][R][K] > 0)
    {
      return dp[L][R][K];
    }
    int ans = process1(boxes, L + 1, R, 0, dp) + (K + 1) * (K + 1);
    for (int i = L + 1; i <= R; i++)
    {
      if (boxes[i] == boxes[L])
      {
        ans = std::max(ans, process1(boxes, L + 1, i - 1, 0, dp) + process1(boxes, i, R, K + 1, dp));
      }
    }
    dp[L][R][K] = ans;
    return ans;
  }

  static int removeBoxes2(vector<int> &boxes)
  {
    int N = boxes.size();
    TtArray dp(N, vector<vector<int>>(N, vector<int>(N)));
    int ans = process2(boxes, 0, N - 1, 0, dp);
    return ans;
  }

  static int process2(vector<int> &boxes, int L, int R, int K, TtArray dp)
  {
    if (L > R)
    {
      return 0;
    }
    if (dp[L][R][K] > 0)
    {
      return dp[L][R][K];
    }
    // 找到开头，
    // 1,1,1,1,1,5
    // 3 4 5 6 7 8
    //         !
    int last = L;
    while (last + 1 <= R && boxes[last + 1] == boxes[L])
    {
      last++;
    }
    // K个1     (K + last - L) last
    int pre = K + last - L;
    int ans = (pre + 1) * (pre + 1) + process2(boxes, last + 1, R, 0, dp);
    for (int i = last + 2; i <= R; i++)
    {
      if (boxes[i] == boxes[L] && boxes[i - 1] != boxes[L])
      {
        ans = std::max(ans, process2(boxes, last + 1, i - 1, 0, dp) + process2(boxes, i, R, pre + 1, dp));
      }
    }
    dp[L][R][K] = ans;
    return ans;
  }
};

int main()
{
  vector<int> sample{1, 3, 2, 2, 2, 3, 4, 3, 1};
  cout << RemoveBoxes::func1(sample, 0, sample.size() - 1, 0) << endl;
  cout << RemoveBoxes::removeBoxes1(sample) << endl;
  cout << RemoveBoxes::removeBoxes2(sample) << endl;
  return 0;
}
