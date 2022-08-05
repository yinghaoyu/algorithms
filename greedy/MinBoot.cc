#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

// 给定一个正数数组arr，代表若干人的体重
// 再给定一个正数limit，表示所有船共同拥有的载重量
// 每艘船最多坐两人，且不能超过载重
// 想让所有的人同时过河，并且用最好的分配方法让船尽量少
// 返回最少的船数
// 分析：
// × × √ √ √ √ √ × × × × √ √ √ √ √ ~
// 1 1 1 1 1 2 3 5 5 5 5 6 6 8 8 9 10
//                     ↑ ↑
//                     L R
//                     L往左走，R往右走
//                     res = (count(x) + 1)/2 + count(√)/2 + count(~)
//                            +1表示向上取整
class MinBoat
{
 public:
  static int minBoat(vector<int> &arr, int limit)
  {
    if (arr.size() == 0)
    {
      return 0;
    }
    int N = arr.size();
    std::sort(arr.begin(), arr.end());
    if (arr[N - 1] > limit)
    {
      return -1;
    }
    int lessR = -1;
    for (int i = N - 1; i >= 0; i--)
    {
      if (arr[i] <= (limit / 2))  // 找到第一个<= limit / 2的数
      {
        lessR = i;
        break;
      }
    }
    if (lessR == -1)
    {
      return N;
    }
    int L = lessR;
    int R = lessR + 1;
    int noUsed = 0;
    while (L >= 0)
    {
      int solved = 0;
      while (R < N && arr[L] + arr[R] <= limit)
      {
        R++;
        solved++;
      }
      if (solved == 0)
      {
        noUsed++;
        L--;
      }
      else
      {
        L = std::max(-1, L - solved);
      }
    }
    int all = lessR + 1;
    int used = all - noUsed;
    int moreUnsolved = (N - all) - used;
    return used + ((noUsed + 1) >> 1) + moreUnsolved;
  }
};
