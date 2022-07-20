#include <iostream>
#include <vector>

using namespace std;

// 问题描述：
// 超级洗衣机
// 假设有 n 台超级洗衣机放在同一排上，
// 开始的时候，每台洗衣机内可能有一定量的衣服，也可能是空的，
// 在每一步操作中，你可以选择任意 m (1 <= m <= n) 台洗衣机，
// 与此同时将每台洗衣机的一件衣服送到相邻的一台洗衣机。

// 给定一个整数数组 machines 代表从左至右每台洗衣机中的衣物数量，
// 请给出能让所有洗衣机中剩下的衣物的数量相等的 最少的操作步数，
// 如果不能使每台洗衣机中衣物的数量相等，则返回 -1 。

// 测试链接 : https://leetcode.com/problems/super-washing-machines/
class SuperWashingMachines
{
 public:
  static int findMinMoves(vector<int> &arr)
  {
    if (arr.size() == 0)
    {
      return 0;
    }
    int size = arr.size();
    int sum = 0;
    // 先计算前缀和
    for (int i = 0; i < size; i++)
    {
      sum += arr[i];
    }
    if (sum % size != 0)  // 不能均分，直返回-1
    {
      return -1;
    }
    int avg = sum / size;  // 每台洗衣机经过调整后的最终衣服数
    int leftSum = 0;
    int ans = 0;
    for (int i = 0; i < arr.size(); i++)
    {
      // 考虑i左右两边到达最终avg需要调整的数量，正数表示多出的，负数表示缺少的
      // 情况一：
      // left  i  right
      // -15   x    +10  --> right往i左移动10件，i往left左移15件，可以同时进行，取最大值15

      // 情况二：
      // left  i  right
      // +15   x    -10  --> left往i左移动15件，i往right左移10件，可以同时进行，取最大值15

      // 情况三：
      // left i right
      // -15  x  -10    --> i往left左移动15件，i往right右移10件，i只能单步进行，取15+10

      // 情况四：
      // left i right
      // +15  x  +10    --> left往i右移15件，right往i左移10件，可以同时进行，取最大值15

      // ans 取所有阶段的最大值，因为当最大值调整完毕的时候，其他阶段也一定调整完毕了
      int leftRest = leftSum - i * avg;
      int rightRest = (sum - leftSum - arr[i]) - (size - i - 1) * avg;
      if (leftRest < 0 && rightRest < 0)
      {
        ans = std::max(ans, std::abs(leftRest) + std::abs(rightRest));
      }
      else
      {
        ans = std::max(ans, std::max(std::abs(leftRest), std::abs(rightRest)));
      }
      leftSum += arr[i];
    }
    return ans;
  }
};

int main()
{
  return 0;
}
