#include <iostream>
#include <vector>

using namespace std;
using TcArray = vector<vector<char>>;
using TbArray = vector<vector<bool>>;

class SudokuSolver
{
 public:
  static void solveSudoku(TcArray &board)
  {
    TbArray row(9, vector<bool>(10));
    TbArray col(9, vector<bool>(10));
    TbArray bucket(9, vector<bool>(10));
    initMaps(board, row, col, bucket);
    process(board, 0, 0, row, col, bucket);
  }

  static void initMaps(TcArray &board, TbArray &row, TbArray &col, TbArray &bucket)
  {
    for (int i = 0; i < 9; i++)
    {
      for (int j = 0; j < 9; j++)
      {
        int bid = 3 * (i / 3) + (j / 3);
        if (board[i][j] != '.')
        {
          int num = board[i][j] - '0';
          row[i][num] = true;
          col[j][num] = true;
          bucket[bid][num] = true;
        }
      }
    }
  }

  //  当前来到(i,j)这个位置，如果已经有数字，跳到下一个位置上
  //                      如果没有数字，尝试1~9，不能和row、col、bucket冲突
  static bool process(TcArray &board, int i, int j, TbArray &row, TbArray &col, TbArray &bucket)
  {
    if (i == 9)
    {
      return true;
    }
    // 当离开(i，j)，应该去哪？(nexti, nextj)
    int nexti = j != 8 ? i : i + 1;
    int nextj = j != 8 ? j + 1 : 0;
    if (board[i][j] != '.')
    {
      // 当前是数字，直接尝试下一个位置
      return process(board, nexti, nextj, row, col, bucket);
    }
    else
    {
      // 可以尝试1~9
      int bid = 3 * (i / 3) + (j / 3);
      for (int num = 1; num <= 9; num++)
      {  // 尝试每一个数字1~9
        if ((!row[i][num]) && (!col[j][num]) && (!bucket[bid][num]))
        {
          // 可以尝试num
          row[i][num] = true;
          col[j][num] = true;
          bucket[bid][num] = true;
          board[i][j] = (char) (num + '0');
          if (process(board, nexti, nextj, row, col, bucket))
          {
            return true;
          }
          row[i][num] = false;
          col[j][num] = false;
          bucket[bid][num] = false;
          board[i][j] = '.';
        }
      }
      return false;
    }
  }
};

int main()
{
  return 0;
}
