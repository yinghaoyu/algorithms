#include <iostream>
#include <random>
#include <vector>

using namespace std;
using TdArray = vector<vector<int>>;

class NCardsABWin
{
  // 谷歌面试题
  // 面值为1~10的牌组成一组，
  // 每次你从组里等概率的抽出1~10中的一张
  // 下次抽会换一个新的组，有无限组
  // 当累加和<17时，你将一直抽牌
  // 当累加和>=17且<21时，你将获胜
  // 当累加和>=21时，你将失败
  // 返回获胜的概率

 public:
  static double f1() { return p1(0); }

  // 游戏的规则，如上
  // 当你来到cur这个累加和的时候，获胜概率是多少返回！
  static double p1(int cur)
  {
    if (cur >= 17 && cur < 21)
    {
      return 1.0;
    }
    if (cur >= 21)
    {
      return 0.0;
    }
    double w = 0.0;
    for (int i = 1; i <= 10; i++)
    {
      w += p1(cur + i);
    }
    return w / 10;  // 从10张卡抽出1张的概率为1/10
  }

  // 谷歌面试题扩展版
  // 面值为1~N的牌组成一组，
  // 每次你从组里等概率的抽出1~N中的一张
  // 下次抽会换一个新的组，有无限组
  // 当累加和<a时，你将一直抽牌
  // 当累加和>=a且<b时，你将获胜
  // 当累加和>=b时，你将失败
  // 返回获胜的概率，给定的参数为N，a，b
  static double f2(int N, int a, int b)
  {
    if (N < 1 || a >= b || a < 0 || b < 0)
    {
      return 0.0;
    }
    // 假设N=3，a=15，b=20
    // 14 15 16 17 18 19 20
    // 当你在14时，就算你抽的是最大值3，14+3=17，你必赢
    if (b - a >= N)
    {
      return 1.0;
    }
    // 所有参数都合法，并且b-a < N
    return p2(0, N, a, b);
  }

  // 游戏规则，如上，int N, int a, int b，固定参数！
  // cur，目前到达了cur的累加和
  // 返回赢的概率
  static double p2(int cur, int N, int a, int b)
  {
    if (cur >= a && cur < b)
    {
      return 1.0;
    }
    if (cur >= b)
    {
      return 0.0;
    }
    double w = 0.0;
    for (int i = 1; i <= N; i++)
    {
      w += p2(cur + i, N, a, b);
    }
    return w / N;
  }

  // f2的改进版本，用到了观察位置优化枚举的技巧
  static double f3(int N, int a, int b)
  {
    if (N < 1 || a >= b || a < 0 || b < 0)
    {
      return 0.0;
    }
    if (b - a >= N)
    {
      return 1.0;
    }
    return p3(0, N, a, b);
  }

  static double p3(int cur, int N, int a, int b)
  {
    if (cur >= a && cur < b)
    {
      return 1.0;
    }
    if (cur >= b)
    {
      return 0.0;
    }
    // 假设N=6，a=18，b=20
    // 10  11  12  13  14  15  16  17  18  19  20  21  22  23
    //                            0.5   1   1   1   0   0   0
    if (cur == a - 1)  // 计算边界值17
    {
      return 1.0 * (b - a) / N;
    }
    double w = p3(cur + 1, N, a, b) + p3(cur + 1, N, a, b) * N;
    if (cur + 1 + N < b)  // 计算<=14的概率
    {
      w -= p3(cur + 1 + N, N, a, b);
    }
    return w / N;
  }

  // f3的改进版本的动态规划
  static double f4(int N, int a, int b)
  {
    if (N < 1 || a >= b || a < 0 || b < 0)
    {
      return 0.0;
    }
    if (b - a >= N)
    {
      return 1.0;
    }
    vector<double> dp(b);
    for (int i = a; i < b; i++)
    {
      dp[i] = 1.0;
    }
    if (a - 1 >= 0)
    {
      dp[a - 1] = 1.0 * (b - a) / N;
    }
    for (int cur = a - 2; cur >= 0; cur--)
    {
      double w = dp[cur + 1] + dp[cur + 1] * N;
      if (cur + 1 + N < b)
      {
        w -= dp[cur + 1 + N];
      }
      dp[cur] = w / N;
    }
    return dp[0];
  }

  static int getRandom(int min, int max)
  {
    random_device seed;                            // 硬件生成随机数种子
    ranlux48 engine(seed());                       // 利用种子生成随机数引
    uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
    int res = distrib(engine);                     // 随机数
    return res;
  }

  static bool equals(double a, double b) { return a - b < 1e-4 ? true : false; }

  static void test()
  {
    int N = 10;
    int a = 17;
    int b = 21;
    cout << "N = " << N << ", a = " << a << ", b = " << b << endl;
    cout << f1() << endl;
    cout << f2(N, a, b) << endl;
    cout << f3(N, a, b) << endl;
    cout << f4(N, a, b) << endl;

    int maxN = 15;
    int maxM = 20;
    int testTime = 100000;
    cout << "测试开始" << endl;
    cout << "比对double类型答案可能会有精度对不准的问题" << endl;
    cout << "所以答案一律只保留小数点后四位进行比对" << endl;
    cout << "如果没有错误提示, 说明验证通过" << endl;
    for (int i = 0; i < testTime; i++)
    {
      N = getRandom(0, maxN);
      a = getRandom(0, maxM);
      b = getRandom(0, maxM);
      double ans2 = f2(N, a, b);
      double ans3 = f3(N, a, b);
      double ans4 = f4(N, a, b);
      if (!equals(ans2, ans3) || !equals(ans2, ans4))
      {
        cout << "Oops!" << endl;
        cout << N << " , " << a << " , " << b << endl;
        cout << ans2 << endl;
        cout << ans3 << endl;
        cout << ans4 << endl;
      }
    }
    cout << "测试结束" << endl;

    N = 10000;
    a = 67834;
    b = 72315;
    cout << "N = " << N << ", a = " << a << ", b = " << b << "时, 除了方法4外都超时" << endl;
    cout << "方法4答案: ";
    cout << f4(N, a, b) << endl;
  }
};

int main()
{
  NCardsABWin::test();
  return 0;
}
