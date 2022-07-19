#include <iostream>
#include <string>
#include <vector>

using namespace std;
using TtArray = vector<vector<vector<int>>>;

class BooleanEvaluation
{
 public:
  static int countEval0(string express, int desired)
  {
    if (express.length() == 0)
    {
      return 0;
    }
    int N = express.length();
    vector<vector<Info>> dp(N, vector<Info>(N));
    Info allInfo = func(express, 0, express.length() - 1, dp);
    return desired == 1 ? allInfo.t : allInfo.f;
  }

  class Info
  {
   public:
    int t;
    int f;

    Info(int tr, int fa)
    {
      t = tr;
      f = fa;
    }

    Info()
    {
      t = -1;
      f = -1;
    }

    bool operator!=(const Info &other) { return this->t != other.t || this->f != other.f; }
  };

  // 限制:
  // L...R上，一定有奇数个字符
  // L位置的字符和R位置的字符，非0即1，不能是逻辑符号！
  // 返回str[L...R]这一段，为true的方法数，和false的方法数
  static Info func(string str, int L, int R, vector<vector<Info>> &dp)
  {
    if (dp[L][R] != Info())
    {
      return dp[L][R];
    }
    int t = 0;
    int f = 0;
    if (L == R)
    {
      t = str[L] == '1' ? 1 : 0;
      f = str[L] == '0' ? 1 : 0;
    }
    else
    {  // L..R >=3
      // 每一个种逻辑符号，split枚举的东西
      // 都去试试最后结合
      for (int split = L + 1; split < R; split += 2)
      {
        Info leftInfo = func(str, L, split - 1, dp);
        Info rightInfo = func(str, split + 1, R, dp);
        int a = leftInfo.t;
        int b = leftInfo.f;
        int c = rightInfo.t;
        int d = rightInfo.f;
        switch (str[split])
        {
        case '&':
          t += a * c;
          f += b * c + b * d + a * d;
          break;
        case '|':
          t += a * c + a * d + b * c;
          f += b * d;
          break;
        case '^':
          t += a * d + b * c;
          f += a * c + b * d;
          break;
        }
      }
    }
    dp[L][R] = Info(t, f);
    return dp[L][R];
  }

  static int countEval1(string express, int desired)
  {
    if (express.length() == 0)
    {
      return 0;
    }
    return f(express, desired, 0, express.length() - 1);
  }

  static int f(string str, int desired, int L, int R)
  {
    if (L == R)
    {
      if (str[L] == '1')
      {
        return desired;
      }
      else
      {
        return desired ^ 1;
      }
    }
    int res = 0;
    if (desired == 1)
    {
      for (int i = L + 1; i < R; i += 2)
      {
        switch (str[i])
        {
        case '&':
          res += f(str, 1, L, i - 1) * f(str, 1, i + 1, R);
          break;
        case '|':
          res += f(str, 1, L, i - 1) * f(str, 0, i + 1, R);
          res += f(str, 0, L, i - 1) * f(str, 1, i + 1, R);
          res += f(str, 1, L, i - 1) * f(str, 1, i + 1, R);
          break;
        case '^':
          res += f(str, 1, L, i - 1) * f(str, 0, i + 1, R);
          res += f(str, 0, L, i - 1) * f(str, 1, i + 1, R);
          break;
        }
      }
    }
    else
    {
      for (int i = L + 1; i < R; i += 2)
      {
        switch (str[i])
        {
        case '&':
          res += f(str, 0, L, i - 1) * f(str, 1, i + 1, R);
          res += f(str, 1, L, i - 1) * f(str, 0, i + 1, R);
          res += f(str, 0, L, i - 1) * f(str, 0, i + 1, R);
          break;
        case '|':
          res += f(str, 0, L, i - 1) * f(str, 0, i + 1, R);
          break;
        case '^':
          res += f(str, 1, L, i - 1) * f(str, 1, i + 1, R);
          res += f(str, 0, L, i - 1) * f(str, 0, i + 1, R);
          break;
        }
      }
    }
    return res;
  }

  static int countEval2(string express, int desired)
  {
    if (express.length() == 0)
    {
      return 0;
    }
    int N = express.length();
    TtArray dp(2, vector<vector<int>>(N, vector<int>(N)));
    // dp[0][j][i]表示[j...i]能组成false的方法数
    // dp[1][j][i]表示[j...i]能组成true的方法数
    dp[0][0][0] = express[0] == '0' ? 1 : 0;
    dp[1][0][0] = dp[0][0][0] ^ 1;
    for (int i = 2; i < express.length(); i += 2)
    {
      dp[0][i][i] = express[i] == '1' ? 0 : 1;
      dp[1][i][i] = express[i] == '0' ? 0 : 1;
      for (int j = i - 2; j >= 0; j -= 2)
      {
        for (int k = j; k < i; k += 2)
        {
          if (express[k + 1] == '&')
          {
            dp[1][j][i] += dp[1][j][k] * dp[1][k + 2][i];
            dp[0][j][i] += (dp[0][j][k] + dp[1][j][k]) * dp[0][k + 2][i] + dp[0][j][k] * dp[1][k + 2][i];
          }
          else if (express[k + 1] == '|')
          {
            dp[1][j][i] += (dp[0][j][k] + dp[1][j][k]) * dp[1][k + 2][i] + dp[1][j][k] * dp[0][k + 2][i];
            dp[0][j][i] += dp[0][j][k] * dp[0][k + 2][i];
          }
          else
          {
            dp[1][j][i] += dp[0][j][k] * dp[1][k + 2][i] + dp[1][j][k] * dp[0][k + 2][i];
            dp[0][j][i] += dp[0][j][k] * dp[0][k + 2][i] + dp[1][j][k] * dp[1][k + 2][i];
          }
        }
      }
    }
    return dp[desired][0][N - 1];
  }
};

int main()
{
  return 0;
}
