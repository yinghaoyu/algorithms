#include <iostream>

using namespace std;

// 返回 >= n 的最小的2的幂
class Near2power
{
  int near2power(int n)
  {
    if(n <= 0)
    {
      return 1;
    }
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 8;
    n |= n >> 16;
    return n + 1;
  }
};
