#include <iostream>
#include <vector>
using namespace std;
using TdArray = vector<vector<int>>;

// 问题描述：最佳碰头地点
// 把二维数组所有1都聚集在数组内某一个点上，这些1只能上、下、左、右4个方向走动，求所有点数的最短距离和
//
// 分析：
// 先统计行的最优，再统计列的最优
//
// 例子：
//  1 1 1 1 1 1 0 0 0 1
//  1 1 1 0 0 0 0 0 0 0    --> 上下往中间统计，谁的累加小，就不用这行
//  1 0 0 0 0 0 0 0 0 0        第0行7个1，第3行5个1，7 > 5那么答案一定不在第3行，下方取第2行累加第3行的1
//  0 0 0 0 1 1 1 1 1 1        第0行7个1，第3和第2行累加6个1，7 > 6那么答案一定不在第2行
//  因为上、下两方谁的累加和大，谁需要调整的距离就越大！

// 测试链接：https://leetcode.cn/problems/best-meeting-point/

class BestMeetingPoint
{
 public:
  static int minTotalDistance(TdArray &grid)
  {
    int N = grid.size();
    int M = grid[0].size();
    vector<int> iOnes(N);  // 统计每行的1
    vector<int> jOnes(M);  // 统计每列的1
    for (int i = 0; i < N; i++)
    {
      for (int j = 0; j < M; j++)
      {
        if (grid[i][j] == 1)
        {
          iOnes[i]++;
          jOnes[j]++;
        }
      }
    }
    int total = 0;
    int i = 0;
    int j = N - 1;
    int iRest = 0;  // 行1的累加和
    int jRest = 0;  // 列1的累加和
    while (i < j)   // 行统计，上行、下行往中间缩
    {
      if (iOnes[i] + iRest <= iOnes[j] + jRest)
      {
        // 上行更小，行号往下移动，结算，同时计算累加1的数量
        total += iOnes[i] + iRest;
        iRest += iOnes[i++];
      }
      else
      {
        // 下行更小，行号往上移动，结算，同时计算累加1的数量
        total += iOnes[j] + jRest;
        jRest += iOnes[j--];
      }
    }
    i = 0;
    j = M - 1;
    iRest = 0;
    jRest = 0;
    while (i < j)  // 同理，进行列统计
    {
      if (jOnes[i] + iRest <= jOnes[j] + jRest)
      {
        total += jOnes[i] + iRest;
        iRest += jOnes[i++];
      }
      else
      {
        total += jOnes[j] + jRest;
        jRest += jOnes[j--];
      }
    }
    return total;
  }
};

int main()
{
  return 0;
}
