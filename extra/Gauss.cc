#include <cmath>
#include <iostream>

using namespace std;

//@sa https://www.bilibili.com/list/8888480/
// 高斯消元

const int MAXN = 101;
double mat[MAXN][MAXN];
const double sml = 1e-7;

void swapRows(int a, int b)
{
  for (int j = 0; j < MAXN; ++j)
  {
    swap(mat[a][j], mat[b][j]);
  }
}

void gauss(int n)
{
  for (int i = 0; i < n; ++i)
  {
    int max = i;
    for (int j = i; j < n; ++j)
    {
      if (fabs(mat[j][i]) > fabs(mat[max][i]))
      {
        max = j;
      }
    }
    swapRows(i, max);

    if (fabs(mat[i][i]) >= sml)
    {
      double tmp = mat[i][i];
      for (int j = i; j <= n; ++j)
      {
        mat[i][j] /= tmp;
      }

      for (int j = 0; j < n; ++j)
      {
        if (i != j)
        {
          double rate = mat[j][i] / mat[i][i];
          for (int k = i; k <= n; ++k)
          {
            mat[j][k] -= mat[i][k] * rate;
          }
        }
      }
    }
  }
}

void print(int n)
{
  for (int i = 0; i < n; ++i)
  {
    for (int j = 0; j <= n; ++j)
    {
      printf("%.2f  ", mat[i][j]);
    }
    cout << endl;
  }
  cout << "========================" << endl;
}

int main()
{
  // 唯一解
  cout << "唯一解" << endl;
  mat[0][0] = 1;
  mat[0][1] = 2;
  mat[0][2] = -1;
  mat[0][3] = 9;
  mat[1][0] = 2;
  mat[1][1] = -1;
  mat[1][2] = 2;
  mat[1][3] = 7;
  mat[2][0] = 1;
  mat[2][1] = -2;
  mat[2][2] = 2;
  mat[2][3] = 0;
  gauss(3);
  print(3);

  // 矛盾
  cout << "矛盾" << endl;
  mat[0][0] = 1;
  mat[0][1] = 1;
  mat[0][2] = 3;
  mat[1][0] = 2;
  mat[1][1] = 2;
  mat[1][2] = 7;
  gauss(2);
  print(2);

  // 多解
  cout << "多解" << endl;
  mat[0][0] = 1;
  mat[0][1] = 1;
  mat[0][2] = 3;
  mat[1][0] = 2;
  mat[1][1] = 2;
  mat[1][2] = 6;
  gauss(2);
  print(2);

  // 表达式冗余，唯一解
  cout << "表达式冗余，唯一解" << endl;
  mat[0][0] = 1;
  mat[0][1] = 2;
  mat[0][2] = -1;
  mat[0][3] = 0;
  mat[0][4] = 9;
  mat[1][0] = 2;
  mat[1][1] = 4;
  mat[1][2] = -2;
  mat[1][3] = 0;
  mat[1][4] = 18;
  mat[2][0] = 2;
  mat[2][1] = -1;
  mat[2][2] = 2;
  mat[2][3] = 0;
  mat[2][4] = 7;
  mat[3][0] = 1;
  mat[3][1] = -2;
  mat[3][2] = 2;
  mat[3][3] = 0;
  mat[3][4] = 0;
  gauss(4);
  print(4);

  // 表达式冗余，矛盾
  cout << "表达式冗余，矛盾" << endl;
  mat[0][0] = 1;
  mat[0][1] = 2;
  mat[0][2] = -1;
  mat[0][3] = 0;
  mat[0][4] = 9;
  mat[1][0] = 2;
  mat[1][1] = 4;
  mat[1][2] = -2;
  mat[1][3] = 0;
  mat[1][4] = 18;
  mat[2][0] = 2;
  mat[2][1] = -1;
  mat[2][2] = 2;
  mat[2][3] = 0;
  mat[2][4] = 7;
  mat[3][0] = 4;
  mat[3][1] = -2;
  mat[3][2] = 4;
  mat[3][3] = 0;
  mat[3][4] = 10;
  gauss(4);
  print(4);

  // 表达式冗余，多解
  cout << "表达式冗余，多解" << endl;
  mat[0][0] = 1;
  mat[0][1] = 2;
  mat[0][2] = -1;
  mat[0][3] = 0;
  mat[0][4] = 9;
  mat[1][0] = 2;
  mat[1][1] = 4;
  mat[1][2] = -2;
  mat[1][3] = 0;
  mat[1][4] = 18;
  mat[2][0] = 2;
  mat[2][1] = -1;
  mat[2][2] = 2;
  mat[2][3] = 0;
  mat[2][4] = 7;
  mat[3][0] = 4;
  mat[3][1] = -2;
  mat[3][2] = 4;
  mat[3][3] = 0;
  mat[3][4] = 14;
  gauss(4);
  print(4);

  // 表达式不足，矛盾
  cout << "表达式不足，矛盾" << endl;
  mat[0][0] = 1;
  mat[0][1] = 2;
  mat[0][2] = -1;
  mat[0][3] = 5;
  mat[1][0] = 2;
  mat[1][1] = 4;
  mat[1][2] = -2;
  mat[1][3] = 7;
  mat[2][0] = 0;
  mat[2][1] = 0;
  mat[2][2] = 0;
  mat[2][3] = 0;
  gauss(3);
  print(3);

  // 表达式不足，多解
  cout << "表达式不足，多解" << endl;
  mat[0][0] = 1;
  mat[0][1] = 2;
  mat[0][2] = -1;
  mat[0][3] = 5;
  mat[1][0] = 2;
  mat[1][1] = 2;
  mat[1][2] = -1;
  mat[1][3] = 8;
  mat[2][0] = 0;
  mat[2][1] = 0;
  mat[2][2] = 0;
  mat[2][3] = 0;
  gauss(3);
  print(3);

  // 正确区分矛盾、多解、唯一解
  cout << "正确区分矛盾、多解、唯一解" << endl;
  mat[0][0] = 0;
  mat[0][1] = 2;
  mat[0][2] = 3;
  mat[1][0] = 0;
  mat[1][1] = 0;
  mat[1][2] = 0;
  gauss(2);
  print(2);

  // 有些主元可以确定值
  cout << "有些主元可以确定值" << endl;
  mat[0][0] = 1;
  mat[0][1] = 1;
  mat[0][2] = 1;
  mat[0][3] = 5;
  mat[1][0] = 2;
  mat[1][1] = 1;
  mat[1][2] = 1;
  mat[1][3] = 7;
  mat[2][0] = 0;
  mat[2][1] = 0;
  mat[2][2] = 0;
  mat[2][3] = 0;
  gauss(3);
  print(3);

  // 有些主元还受到自由元的影响
  cout << "有些主元还受到自由元的影响" << endl;
  mat[0][0] = 1;
  mat[0][1] = 1;
  mat[0][2] = 5;
  mat[1][0] = 0;
  mat[1][1] = 0;
  mat[1][2] = 0;
  gauss(2);
  print(2);

  return 0;
}
