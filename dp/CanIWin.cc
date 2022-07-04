#include <iostream>
#include <vector>

using namespace std;

// 问题描述：
// 在 "100 game" 这个游戏中，
// 两名玩家轮流选择从 1 到 10 的任意整数，
// 累计整数和，先使得累计整数和达到或超过 100 的玩家，即为胜者。
//
// 测试链接：https://leetcode.cn/problems/can-i-win/

class CanIWin
{
 public:
  // 1~choose 拥有的数字
  // total 一开始的剩余
  // 返回先手会不会赢
  static bool canIWin0(int choose, int total)
  {
    if (total == 0)
    {
      return true;
    }
    // 如果所有数加起来都小于total，那么谁也赢不了
    if ((choose * (choose + 1) >> 1) < total)
    {
      return false;
    }
    vector<int> arr(choose);
    for (int i = 0; i < choose; i++)
    {
      arr[i] = i + 1;
    }
    // arr[i] != -1 表示arr[i]这个数字还没被拿走
    // arr[i] == -1 表示arr[i]这个数字已经被拿走
    // 集合，arr，1~choose
    return process(arr, total);
  }

  // 当前轮到先手拿，
  // 先手只能选择在arr中还存在的数字，
  // 还剩rest这个值，
  // 返回先手会不会赢
  static bool process(vector<int> &arr, int rest)
  {
    if (rest <= 0)
    {
      return false;
    }
    // 先手去尝试所有的情况
    for (int i = 0; i < arr.size(); i++)
    {
      if (arr[i] != -1)
      {
        int cur = arr[i];
        arr[i] = -1;
        bool next = process(arr, rest - cur);  // 先手取了cur，next是后手的返回值
        arr[i] = cur;
        if (!next)  // 后手返回false，先手才是true
        {
          return true;
        }
      }
    }
    return false;
  }

  // 这个是暴力尝试，超时而已
  static bool canIWin1(int choose, int total)
  {
    if (total == 0)
    {
      return true;
    }
    if ((choose * (choose + 1) >> 1) < total)
    {
      return false;
    }
    return process1(choose, 0, total);
  }

  // 当前轮到先手拿，
  // 先手可以拿1~choose中的任何一个数字
  // status   i位如果为0，代表没拿，当前可以拿
  //          i位为1，代表已经拿过了，当前不能拿
  // 还剩rest这个值，
  // 返回先手会不会赢
  // 这里要求choose <= 32，用位操作优化数组访问
  static bool process1(int choose, int status, int rest)
  {
    if (rest <= 0)
    {
      return false;
    }
    for (int i = 1; i <= choose; i++)
    {
      if (((1 << i) & status) == 0)
      {
        // i 这个数字，是此时先手的决定！
        if (!process1(choose, (status | (1 << i)), rest - i))
        {
          return true;
        }
      }
    }
    return false;
  }

  // 暴力尝试改动态规划
  static bool canIWin2(int choose, int total)
  {
    if (total == 0)
    {
      return true;
    }
    if ((choose * (choose + 1) >> 1) < total)
    {
      return false;
    }
    vector<int> dp(1 << (choose + 1));
    // dp[status] == 1  true
    // dp[status] == -1  false
    // dp[status] == 0  process(status) 没算过！去算！
    return process2(choose, 0, total, dp);
  }

  // 为什么明明status和rest是两个可变参数，却只用status来代表状态(也就是dp)
  // 因为选了一批数字之后，得到的和一定是一样的，所以rest是由status决定的，所以rest不需要参与记忆  化搜索
  static bool process2(int choose, int status, int rest, vector<int> &dp)
  {
    if (dp[status] != 0)
    {
      return dp[status] == 1 ? true : false;
    }
    bool ans = false;
    if (rest > 0)
    {
      for (int i = 1; i <= choose; i++)
      {
        if (((1 << i) & status) == 0)
        {
          if (!process2(choose, (status | (1 << i)), rest - i, dp))
          {
            ans = true;
            break;
          }
        }
      }
    }
    dp[status] = ans ? 1 : -1;
    return ans;
  }
};

int main()
{
  return 0;
}
