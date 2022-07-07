#include <iostream>
#include <random>
#include <vector>

using namespace std;

// 测试链接：https://leetcode.com/problems/largest-rectangle-in-histogram

// 题目描述：
// 给定 n 个非负整数，用来表示柱状图中各个柱子的高度。每个柱子彼此相邻，且宽度为 1 。
// 求在该柱状图中，能够勾勒出来的矩形的最大面积。
//        _                   _
//      _| |                _| |
//     | | |               |♠|♠|
//     | | |  _            |♠|♠|  _
//  _  | | |_| |  -->   _  |♠|♠|_| |   answer = 10
// | |_| | | | |       | |_|♠|♠| | |
// |_|_|_|_|_|_|       |_|_|♠|♠|_|_|

class LargestRectangleInHistogram
{
 public:
  static int largestRectangleArea1(int *height, int len)
  {
    if (height == nullptr || len == 0)
    {
      return 0;
    }
    int maxArea = 0;
    vector<int> stack;
    for (int i = 0; i < len; i++)
    {
      // 单调递减栈
      while (!stack.empty() && height[i] <= height[stack.back()])
      {
        // j 是区间[k + 1, i]的最小值
        int j = stack.back();
        stack.pop_back();
        int k = stack.empty() ? -1 : stack.back();
        int curArea = (i - k - 1) * height[j];
        maxArea = std::max(maxArea, curArea);
      }
      stack.push_back(i);
    }
    while (!stack.empty())
    {
      int j = stack.back();
      stack.pop_back();
      int k = stack.empty() ? -1 : stack.back();
      int curArea = (len - k - 1) * height[j];
      maxArea = std::max(maxArea, curArea);
    }
    return maxArea;
  }

  static int largestRectangleArea2(int *height, int len)
  {
    if (height == nullptr || len == 0)
    {
      return 0;
    }
    int N = len;
    int *stack = new int[N];
    int si = -1;
    int maxArea = 0;
    for (int i = 0; i < len; i++)
    {
      while (si != -1 && height[i] <= height[stack[si]])
      {
        int j = stack[si--];
        int k = si == -1 ? -1 : stack[si];
        int curArea = (i - k - 1) * height[j];
        maxArea = std::max(maxArea, curArea);
      }
      stack[++si] = i;
    }
    while (si != -1)
    {
      int j = stack[si--];
      int k = si == -1 ? -1 : stack[si];
      int curArea = (len - k - 1) * height[j];
      maxArea = std::max(maxArea, curArea);
    }
    return maxArea;
  }
};
