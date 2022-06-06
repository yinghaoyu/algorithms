#include <iostream>
#include <vector>

using namespace std;

using TdArray = vector<vector<int>>;

class PrintStar
{
 public:
  static void printStar(int N)
  {
    int leftUp = 0;
    int rightDown = N - 1;
    TdArray m(N, vector<int>(N));
    // 初始化
    for (int i = 0; i < N; i++)
    {
      for (int j = 0; j < N; j++)
      {
        m[i][j] = ' ';
      }
    }
    while (leftUp <= rightDown)
    {
      set(m, leftUp, rightDown);
      leftUp += 2;
      rightDown -= 2;
    }
    // 打印
    for (int i = 0; i < N; i++)
    {
      for (int j = 0; j < N; j++)
      {
        cout << static_cast<char>(m[i][j]) << " ";
      }
      cout << endl;
    }
  }

  static void set(TdArray &m, int leftUp, int rightDown)
  {
    // 按 → 打印
    for (int col = leftUp; col <= rightDown; col++)
    {
      m[leftUp][col] = '*';
    }
    // 按 ↓ 打印
    for (int row = leftUp + 1; row <= rightDown; row++)
    {
      m[row][rightDown] = '*';
    }
    // 按 ← 打印
    for (int col = rightDown - 1; col > leftUp; col--)
    {
      m[rightDown][col] = '*';
    }
    // 按 ↑ 打印
    for (int row = rightDown - 1; row > leftUp + 1; row--)
    {
      m[row][leftUp + 1] = '*';
    }
  }

  static void test() { printStar(10); }
};

int main()
{
  PrintStar::test();
  return 0;
}
