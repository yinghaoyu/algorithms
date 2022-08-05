#include <iostream>
#include <vector>

using namespace std;

// 问题描述：
// 长度为n的数组piles，表示有n堆香蕉，piles[i]表示第i堆有几根。警卫已经离开了，将在 h 小时后回来。
// 珂珂可以决定她吃香蕉的速度 k （单位：根/小时）。
// 每个小时，她将会选择一堆香蕉，从中吃掉 k 根。
// 如果这堆香蕉少于 k 根，她将吃掉这堆的所有香蕉，然后这一小时内不会再吃更多的香蕉。  
// 返回她可以在 h 小时内吃掉所有香蕉的最小速度 k（k 为整数）。
//
// 分析：
// 由于加快吃香蕉的速度，一定为让总时长不变或者变小，满足单调性，因此可以用二分查找

class KokoEatingBananas
{
 public:
  static int minEatingSpeed(vector<int> &piles, int h)
  {
    int L = 1;
    int R = 0;
    // 有效速度一定在 1~ max之间，速度太快，无意义
    for (int pile : piles)
    {
      R = std::max(R, pile);
    }
    int ans = 0;
    int M = 0;
    while (L <= R)
    {
      M = L + ((R - L) >> 1);
      if (hours(piles, M) <= h)
      {
        ans = M;
        R = M - 1;
      }
      else
      {
        L = M + 1;
      }
    }
    return ans;
  }

  // 返回以speed的速度吃香蕉要多久
  static long hours(vector<int> &piles, int speed)
  {
    long ans = 0;
    int offset = speed - 1;
    for (int pile : piles)
    {
      ans += (pile + offset) / speed;  // 向上取整
    }
    return ans;
  }
};

int main()
{
  return 0;
}
