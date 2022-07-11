#include <iostream>
#include <vector>

using namespace std;

using TdArray = vector<vector<int>>;

// 测试链接：https://leetcode.com/problems/count-submatrices-with-all-ones
// 题目描述：给你一个 m x n 的二进制矩阵 mat ，请你返回有多少个 子矩形 的元素全部都是 1 。
// 1 0 1
// 1 1 0
// 1 1 0
// 输出：13
// 解释：
// 有 6 个 1x1 的矩形。
// 有 2 个 1x2 的矩形。
// 有 3 个 2x1 的矩形。
// 有 1 个 2x2 的矩形。
// 有 1 个 3x1 的矩形。
// 矩形数目总共 = 6 + 2 + 3 + 1 + 1 = 13 。
//
// 思路：
// 枚举第0行为地基，第1行为地基，第2行为地基…第i行为地基的情况下，有多少个子矩阵内部全是1。
// 每一行求出来的子矩阵一定都不一样，而客观上矩阵一定以某一行做底，所以既不会算多，也不会算少。
// 不会算多是因为以第0行为底的子矩阵一定和以第1行为底的子矩阵都不同；
// 不会算少是因为任何一个达标的子矩阵都会以具体的某一行做底。
// 并且相等的时候不算，也是一个相等时候舍弃的问题。

//  _   _
// |_|_|_|  以第一行为地基，结果是2
//
//  _
// | |_
// |_|_|_   以第二行为地基(第一行传递)，结果是4
//
//  _
// | |_
// | | |
// |_|_|_   以第三行为地基(第二行传递)，结果是7

class CountSubmatricesWithAllOnes
{
 public:
  static int numSubmat(TdArray &mat)
  {
    if (mat.size() == 0 || mat[0].size() == 0)
    {
      return 0;
    }
    int nums = 0;
    vector<int> height(mat[0].size());
    for (int i = 0; i < mat.size(); i++)
    {
      for (int j = 0; j < mat[0].size(); j++)
      {
        height[j] = mat[i][j] == 0 ? 0 : height[j] + 1;
      }
      nums += countFromBottom(height);
    }
    return nums;
  }

  static int countFromBottom(vector<int> height)
  {
    if (height.size() == 0)
    {
      return 0;
    }
    int nums = 0;
    vector<int> stack(height.size());
    int si = -1;
    for (int i = 0; i < height.size(); i++)
    {
      // 构建单调递增栈，不符合时弹出结算
      while (si != -1 && height[stack[si]] >= height[i])
      {
        // height[cur] 是区间 [left, i)之间的最小值
        int cur = stack[si--];
        if (height[cur] > height[i])  // 相等的时候舍弃，不然会重复计算
        {
          int left = si == -1 ? -1 : stack[si];
          // 形成直方图宽度n
          int n = i - left - 1;
          int down = std::max(left == -1 ? 0 : height[left], height[i]);
          // 形成的直方图有效结算高度height[cur] - down
          // l = left，c = cur
          // 计算必须以最后一行为底，且一定包含♠的矩阵
          //      _                _                _                 _
          //    _|♠|             _|*|             _|*|              _|*|
          //   |*|*|_           |♠|♠|_           |*|*|_            |*|*|_
          //  _|*|*|*|         _|*|*|*|         _|♠|♠|♠|          _|*|*|*|
          // |*|*|*|*|   -->  |*|*|*|*|   -->  |*|*|*|*|    -->  |♠|♠|♠|♠|
          // |*|*|*|*|        |*|*|*|*|        |*|*|*|*|         |♠|♠|♠|♠|
          //  ↑   ↑ ↑          ↑ ↑   ↑          ↑     ↑           ↑
          //  l   c i          l c   i          l     c           c
          nums += (height[cur] - down) * num(n);
        }
      }
      stack[++si] = i;
    }
    while (si != -1)
    {
      int cur = stack[si--];
      int left = si == -1 ? -1 : stack[si];
      int n = height.size() - left - 1;
      int down = left == -1 ? 0 : height[left];
      nums += (height[cur] - down) * num(n);
    }
    return nums;
  }

  static int num(int n) { return ((n * (1 + n)) >> 1); }
};

int main()
{
  TdArray sample{{1, 0, 1}, {1, 1, 0}, {1, 1, 0}};
  cout << CountSubmatricesWithAllOnes::numSubmat(sample) << endl;
}
