#include <iostream>
#include <vector>

using namespace std;
using TdArray = vector<vector<int>>;

// 测试链接：https://leetcode.cn/problems/game-of-life/comments/
// 康威的生命游戏：https://www.bilibili.com/video/BV1rJ411n7ri

class GameOfLife
{
 public:
  static void gameOfLife(TdArray &board)
  {
    int N = board.size();
    int M = board[0].size();
    for (int i = 0; i < N; i++)
    {
      for (int j = 0; j < M; j++)
      {
        int neighbors = neighbors(board, i, j);
        if (neighbors == 3 || (board[i][j] == 1 && neighbors == 2))
        {
          board[i][j] |= 2;
        }
      }
    }
    for (int i = 0; i < N; i++)
    {
      for (int j = 0; j < M; j++)
      {
        board[i][j] >>= 1;
      }
    }
  }

  // b[i][j] 这个位置的数，周围有几个1
  static int neighbors(TdArray &b, int i, int j)
  {
    return f(b, i - 1, j - 1) + f(b, i - 1, j) + f(b, i - 1, j + 1) + f(b, i, j - 1) + f(b, i, j + 1) + f(b, i + 1, j - 1) + f(b, i + 1, j) +
           f(b, i + 1, j + 1);
  }

  // b[i][j] 上面有1，就返回1，上面不是1，就返回0
  static int f(TdArray &b, int i, int j) { return (i >= 0 && i < b.size() && j >= 0 && j < b[0].size() && (b[i][j] & 1) == 1) ? 1 : 0; }
};

int main()
{
  return 0;
}
