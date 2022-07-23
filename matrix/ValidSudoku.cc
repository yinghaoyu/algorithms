#include <iostream>
#include <vector>

using namespace std;

using TcArray = vector<vector<char>>;
using TbArray = vector<vector<bool>>;

// 问题分析：
// 请你判断一个 9 x 9 的数独是否有效。只需要 根据以下规则 ，验证已经填入的数字是否有效即可。
// 数字 1-9 在每一行只能出现一次。
// 数字 1-9 在每一列只能出现一次。
// 数字 1-9 在每一个以粗实线分隔的 3x3 宫内只能出现一次。

// 测试链接：https://leetcode.cn/problems/valid-sudoku/

class ValidSudoku
{
 public:
  static bool isValidSudoku(TcArray &board)
  {
    TbArray row(9, vector<bool>(10));     // 是否在行内出现
    TbArray col(9, vector<bool>(10));     // 是否在列内出现
    TbArray bucket(9, vector<bool>(10));  // 是否在桶内出现
    for (int i = 0; i < 9; i++)
    {
      for (int j = 0; j < 9; j++)
      {
        // 计算在哪个桶子里
        int bid = 3 * (i / 3) + (j / 3);
        if (board[i][j] != '.')
        {
          int num = board[i][j] - '0';
          if (row[i][num] || col[j][num] || bucket[bid][num])
          {
            return false;
          }
          row[i][num] = true;
          col[j][num] = true;
          bucket[bid][num] = true;
        }
      }
    }
    return true;
  }
};

int main()
{
  return 0;
}
