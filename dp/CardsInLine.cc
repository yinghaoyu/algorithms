#include <iostream>
#include <vector>

using namespace std;

using TdArray = vector<vector<int>>;

//给定一个正数整型数组arr，代表数值不同的纸牌排成一条线。
//玩家A和玩家B依次拿走每张纸牌（可以看见所有的牌），规定玩家A先拿，玩家B后拿。
//但是每个玩家每次只能拿走最左或最右的纸牌。
//玩家A和玩家B都绝顶聪明，请返回最后获胜者的分数。

class CardsInLine
{
 public:
  // 根据规则，返回获胜者的分数
  static int win1(int arr[], int len)
  {
    if (arr == nullptr || len == 0)
    {
      return 0;
    }
    int first = f1(arr, 0, len - 1);
    int second = g1(arr, 0, len - 1);
    return std::max(first, second);
  }

  // arr[L..R]，先手获得的最好分数返回
  static int f1(int arr[], int L, int R)
  {
    if (L == R)  // 表明现在只剩下一张牌了
    {
      return arr[L];
    }
    int p1 = arr[L] + g1(arr, L + 1, R);  // 先手先拿左边或右边的牌，然后后手拿arr[L+1 ... R]
    int p2 = arr[R] + g1(arr, L, R - 1);
    return std::max(p1, p2);  // 先手一定取的更大分数
  }

  // // arr[L..R]，后手获得的最好分数返回
  static int g1(int arr[], int L, int R)
  {
    if (L == R)  // 这张牌已被先手拿走
    {
      return 0;
    }
    int p1 = f1(arr, L + 1, R);  // 对手拿走了L位置的数，这时在arr[L+1 ... R]拿牌相当于先手
    int p2 = f1(arr, L, R - 1);  // 对手拿走了R位置的数，这时在arr[L+1 ... R]拿牌相当于先手
    return std::min(p1, p2);     // 先手已经把更大的分数拿走了，现在只剩小的分数
  }

  static int win2(int arr[], int len)
  {
    if (arr == nullptr || len == 0)
    {
      return 0;
    }
    int N = len;
    TdArray fmap(N, vector<int>(N));
    TdArray gmap(N, vector<int>(N));
    for (int i = 0; i < N; i++)
    {
      for (int j = 0; j < N; j++)
      {
        fmap[i][j] = -1;
        gmap[i][j] = -1;
      }
    }
    int first = f2(arr, 0, len - 1, fmap, gmap);
    int second = g2(arr, 0, len - 1, fmap, gmap);
    return std::max(first, second);
  }

  // arr[L..R]，先手获得的最好分数返回
  static int f2(int arr[], int L, int R, TdArray &fmap, TdArray &gmap)
  {
    if (fmap[L][R] != -1)  // 傻缓存减枝
    {
      return fmap[L][R];
    }
    int ans = 0;
    if (L == R)
    {
      ans = arr[L];
    }
    else
    {
      int p1 = arr[L] + g2(arr, L + 1, R, fmap, gmap);
      int p2 = arr[R] + g2(arr, L, R - 1, fmap, gmap);
      ans = std::max(p1, p2);
    }
    fmap[L][R] = ans;
    return ans;
  }

  // // arr[L..R]，后手获得的最好分数返回
  static int g2(int arr[], int L, int R, TdArray &fmap, TdArray &gmap)
  {
    if (gmap[L][R] != -1)
    {
      return gmap[L][R];
    }
    int ans = 0;
    if (L != R)
    {
      int p1 = f2(arr, L + 1, R, fmap, gmap);  // 对手拿走了L位置的数
      int p2 = f2(arr, L, R - 1, fmap, gmap);  // 对手拿走了R位置的数
      ans = std::min(p1, p2);
    }
    gmap[L][R] = ans;
    return ans;
  }

  // 假设arr = {7, 4, 16, 15, 1}
  // 取纵坐标为L，横坐标为R
  // fmap  0  1  2  3  4 gmap 0  1  2  3  4
  //    |-----------------|------------------|
  //   0|  7              |   0              |
  //    |                 |                  |
  //   1|  ×  4           |   ×  0           |  L <= R 因此无效空间填x
  //    |                 |                  |
  //   2|  ×  ×  16       |   ×  ×  0        |
  //    |                 |                  |
  //   3|  ×  ×  ×  15    |   ×  ×  ×  0     |
  //    |                 |                  |
  //   4|  ×  ×  ×  ×  1  |   ×  ×  ×  ×  0  |
  // fmap(L, R)依赖arr[L] + gmap(L+1, R)和arr[R] + gmap(L, R-1)的最大值
  // gmap(L, R)依赖fmap(L+1, R)和fmap(L, R-1)的最小值
  // 根据规律，可以按对角线从左往右填
  static int win3(int arr[], int len)
  {
    if (arr == nullptr || len == 0)
    {
      return 0;
    }
    int N = len;
    TdArray fmap(N, vector<int>(N));
    TdArray gmap(N, vector<int>(N));
    for (int i = 0; i < N; i++)
    {
      fmap[i][i] = arr[i];  // base case
    }
    for (int startCol = 1; startCol < N; startCol++)
    {
      int L = 0;
      int R = startCol;
      while (R < N)  // 按对角线填
      {
        fmap[L][R] = std::max(arr[L] + gmap[L + 1][R], arr[R] + gmap[L][R - 1]);
        gmap[L][R] = std::min(fmap[L + 1][R], fmap[L][R - 1]);
        L++;
        R++;
      }
    }
    return std::max(fmap[0][N - 1], gmap[0][N - 1]);
  }
};

int main()
{
  int arr[] = {5, 7, 4, 5, 8, 1, 6, 0, 3, 4, 6, 1, 7};
  int len = sizeof(arr) / sizeof(int);
  cout << CardsInLine::win1(arr, len) << endl;
  cout << CardsInLine::win2(arr, len) << endl;
  cout << CardsInLine::win3(arr, len) << endl;
  return 0;
}
