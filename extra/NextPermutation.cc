#include <iostream>
#include <vector>

using namespace std;

// 问题描述：
// 找到全排列的下一个排列
//
// 分析：
// 1 2 3 4 7 6 5
//
// 已知降序是最大的一个排列、
//
// 从右往左遍历，找到第一个arr[i] < arr[i+1]的位置
//     7
//   ↙   ↘
// 4       6            --> 这里是 4，在从右边7 6 5 找到第一个>4的交换，
//           ↘              因为4 7 6 5是以4开始的最大全排列，下一个排列一定是第一个>4的数开始的，这里是5
//             5            此时交换后 5 7 6 4是以5开始的最大全排列，只需要逆序为5 4 6 7 即为5开始的最小排列

// 测试链接：https://leetcode-cn.com/problems/next-permutation/

class NextPermutation
{
 public:
  static void nextPermutation(vector<int> &nums)
  {
    int N = nums.size();
    // 从右往左第一次降序的位置
    int firstLess = -1;
    for (int i = N - 2; i >= 0; i--)
    {
      if (nums[i] < nums[i + 1])
      {
        firstLess = i;
        break;
      }
    }
    if (firstLess < 0)
    {
      reverse(nums, 0, N - 1);
    }
    else
    {
      int rightClosestMore = -1;
      // 找最靠右的、同时比nums[firstLess]大的数，位置在哪
      // 这里其实也可以用二分优化，但是这种优化无关紧要了
      for (int i = N - 1; i > firstLess; i--)
      {
        if (nums[i] > nums[firstLess])
        {
          rightClosestMore = i;
          break;
        }
      }
      swap(nums, firstLess, rightClosestMore);
      reverse(nums, firstLess + 1, N - 1);
    }
  }

  static void reverse(vector<int> nums, int L, int R)
  {
    while (L < R)
    {
      swap(nums, L++, R--);
    }
  }

  static void swap(vector<int> &nums, int i, int j)
  {
    int tmp = nums[i];
    nums[i] = nums[j];
    nums[j] = tmp;
  }
};

int main()
{
  return 0;
}
