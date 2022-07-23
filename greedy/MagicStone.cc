#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;
using TdArray = vector<vector<int>>;

// [0,4,7] ： 0表示这里石头没有颜色，如果变红代价是4，如果变蓝代价是7
// [1,X,X] ： 1表示这里石头已经是红，而且不能改颜色，所以后两个数X无意义
// [2,X,X] ： 2表示这里石头已经是蓝，而且不能改颜色，所以后两个数X无意义
// 颜色只可能是0、1、2，代价一定>=0
// 给你一批这样的小数组，要求最后必须所有石头都有颜色，且红色和蓝色一样多，返回最小代价
// 如果怎么都无法做到所有石头都有颜色、且红色和蓝色一样多，返回-1
class MagicStone
{
 public:
  class cmp
  {
   public:
    // 排序规则，第0列全为0，就按第1列和第2列差值从大到小排序
    // 否则就按，无色、红色、蓝色排序
    bool operator()(vector<int> &a, vector<int> &b)
    {
      // 这种写法是错的!
      // a[0] == b[0] ? (a[1] - a[2] - b[1] + b[2]) : (a[0] - b[0])
      // 注意，这里必须用 >，bool值非0即为真，负数也为真!
      bool res = a[0] == b[0] ? (a[1] - a[2] > b[1] - b[2]) : (a[0] < b[0]);
      return res;
    }
  };

  static int minCost(TdArray &stones)
  {
    int n = stones.size();
    if ((n & 1) != 0)  // 奇数说明非法
    {
      return -1;
    }
    std::sort(stones.begin(), stones.end(), cmp());
    for (auto v : stones)
    {
      cout << v[0] << " " << v[1] << " " << v[2] << endl;
    }
    int zero = 0;
    int red = 0;
    int blue = 0;
    int cost = 0;
    for (int i = 0; i < n; i++)
    {
      if (stones[i][0] == 0)
      {
        // 统计无色的石头数
        zero++;
        cost += stones[i][1];  // 先全用红色填充，结算一下代价
      }
      else if (stones[i][0] == 1)
      {
        red++;
      }
      else
      {
        blue++;
      }
    }
    if (red > (n >> 1) || blue > (n >> 1))  // 超过1/2，说明非法
    {
      return -1;
    }
    blue = zero - ((n >> 1) - red);  // 理论上蓝色需要填充的石头
    for (int i = 0; i < blue; i++)
    {
      cost += stones[i][2] - stones[i][1];
    }
    return cost;
  }

  static void test()
  {
    TdArray stones = {{1, 5, 3}, {2, 7, 9}, {0, 6, 4}, {0, 7, 9}, {0, 2, 1}, {0, 5, 9}};
    cout << minCost(stones) << endl;
  }
};

int main()
{
  MagicStone::test();
  return 0;
}
