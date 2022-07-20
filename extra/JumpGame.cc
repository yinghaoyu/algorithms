#include <iostream>
#include <vector>

using namespace std;

// 问题描述：
// 给你一个非负整数数组 nums ，你最初位于数组的第一个位置。
// 数组中的每个元素代表你在该位置可以跳跃的最大长度。
// 你的目标是使用最少的跳跃次数到达数组的最后一个位置。
// 假设你总是可以到达数组的最后一个位置。

// 输入: nums = [2,3,1,1,4]
// 输出: 2
// 解释: 跳到最后一个位置的最小跳跃数是 2。
// 从下标为 0 跳到下标为 1 的位置，跳 1 步，然后跳 3 步到达数组的最后一个位置。

// 测试链接 : https://leetcode.com/problems/jump-game-ii/
class JumpGame
{
 public:
  static int jump(vector<int> &arr)
  {
    if (arr.size() == 0)
    {
      return 0;
    }
    int step = 0;
    int cur = 0;
    int next = 0;
    for (int i = 0; i < arr.size(); i++)
    {
      if (cur < i)  // 当前位置不能到达
      {
        step++;      // 跳一步
        cur = next;  // 跳一步能达到的最远位置
      }
      next = std::max(next, i + arr[i]);  // 更新每次多走一步能走的最远距离
    }
    return step;
  }
};

int main()
{
  vector<int> arr{2, 3, 1, 1, 4};
  cout << JumpGame::jump(arr) << endl;
  return 0;
}
