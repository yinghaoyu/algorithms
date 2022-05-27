#include <iostream>
#include <random>
#include <vector>

using namespace std;

// 测试链接：https://leetcode.com/problems/maximal-rectangle/
// 题目描述：
// 给定一个仅包含 0 和 1 、大小为 rows x cols 的二维二进制矩阵
// 找出只包含 1 的最大矩形，并返回其面积。
//
// 1 0 1 0 0         1  0  1  0  0
// 1 0 1 1 1         1  0 (1  1  1)
// 1 1 1 1 1   -->   1  1 (1  1  1)   answer = 6
// 1 0 0 1 0         1  0  0  1  0

class MaximalRectangle
{
 public:
  // 第一行
  //  _   _
  // |_|_| |_ _
  //
  // 第二行(第一行传递)
  //  _   _
  // | | | |_ _
  // |_|_|_|_|_|
  //
  // 第三行(第二行传递)
  //  _   _
  // | | | |_ _
  // | |_| | | |
  // |_|_|_|_|_|
  //
  // 第四行(第三行传递)
  //  _
  // | |   _
  // | |  | |
  // | |  | |
  // |_|__|_|__
  // 对每行进行LargestRectangleInHistogram，取最大值
  static int maximalRectangle(char **map, int row, int column)
  {
    if (map == nullptr || row == 0 || column == 0)
    {
      return 0;
    }
    int maxArea = 0;
    int *height = new int[column]();
    for (int i = 0; i < row; i++)
    {
      for (int j = 0; j < column; j++)
      {
        // 前一行的前缀和传递给下一行
        height[j] = map[i][j] == '0' ? 0 : height[j] + 1;
      }
      maxArea = std::max(maxRecFromBottom(height, column), maxArea);
    }
    return maxArea;
  }

  // height是正方图数组
  static int maxRecFromBottom(int *height, int len)
  {
    if (height == nullptr || len == 0)
    {
      return 0;
    }
    int maxArea = 0;
    vector<int> stack;
    for (int i = 0; i < len; i++)
    {
      // 单调递增栈
      while (!stack.empty() && height[i] <= height[stack.back()])
      {
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
};
