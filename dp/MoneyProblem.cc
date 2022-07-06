#include <iostream>
#include <random>
#include <vector>

using namespace std;

using TdArray = vector<vector<int>>;

class MoneyProblem
{
 public:
  // int[] d d[i]：i号怪兽的武力
  // int[] p p[i]：i号怪兽要求的钱
  // ability 当前你所具有的能力
  // index 来到了第index个怪兽的面前

  // 目前，你的能力是ability，你来到了index号怪兽的面前，如果要通过后续所有的怪兽，
  // 请返回需要花的最少钱数
  // 贿赂之后怪兽的武力被你同化
  static int process1(vector<int> &d, vector<int> &p, int ability, int index)
  {
    if (index == d.size())
    {
      return 0;
    }
    if (ability < d[index])
    {
      return p[index] + process1(d, p, ability + d[index], index + 1);
    }
    else
    {
      // ability >= d[index] 可以贿赂，也可以不贿赂
      return std::min(p[index] + process1(d, p, ability + d[index], index + 1), 0 + process1(d, p, ability, index + 1));
    }
  }

  static int func1(vector<int> &d, vector<int> &p) { return process1(d, p, 0, 0); }

  // 从0....index号怪兽，花的钱，必须严格==money
  // 如果通过不了，返回-1
  // 如果可以通过，返回能通过情况下的最大能力值
  static int process2(vector<int> &d, vector<int> &p, int index, int money)
  {
    if (index == -1)
    {
      // 一个怪兽也没遇到呢
      return money == 0 ? 0 : -1;
    }
    // index >= 0
    // 1) 不贿赂当前index号怪兽
    int preMaxAbility = process2(d, p, index - 1, money);
    int p1 = -1;
    if (preMaxAbility != -1 && preMaxAbility >= d[index])
    {
      p1 = preMaxAbility;
    }
    // 2) 贿赂当前的怪兽 当前的钱 p[index]
    int preMaxAbility2 = process2(d, p, index - 1, money - p[index]);
    int p2 = -1;
    if (preMaxAbility2 != -1)
    {
      p2 = d[index] + preMaxAbility2;
    }
    return std::max(p1, p2);
  }

  static int minMoney2(vector<int> &d, vector<int> &p)
  {
    int allMoney = 0;  // 贿赂所有怪兽需要的钱
    for (int i = 0; i < p.size(); i++)
    {
      allMoney += p[i];
    }
    int N = d.size();
    for (int money = 0; money < allMoney; money++)
    {
      // 既然要求用最小的钱，那就可以从小到大去枚举money
      if (process2(d, p, N - 1, money) != -1)
      {
        return money;
      }
    }
    return allMoney;
  }

  static int func2(vector<int> &d, vector<int> &p)
  {
    int sum = 0;
    for (int num : d)
    {
      sum += num;  // 对怪兽的武力值求和
    }
    TdArray dp(d.size() + 1, vector<int>(sum + 1));
    for (int i = 0; i <= sum; i++)
    {
      dp[0][i] = 0;
    }
    for (int cur = d.size() - 1; cur >= 0; cur--)
    {
      for (int hp = 0; hp <= sum; hp++)
      {
        // 如果这种情况发生，那么这个hp必然是递归过程中不会出现的状态
        // 既然动态规划是尝试过程的优化，尝试过程碰不到的状态，不必计算
        if (hp + d[cur] > sum)
        {
          continue;
        }
        if (hp < d[cur])
        {
          dp[cur][hp] = p[cur] + dp[cur + 1][hp + d[cur]];
        }
        else
        {
          dp[cur][hp] = std::min(p[cur] + dp[cur + 1][hp + d[cur]], dp[cur + 1][hp]);
        }
      }
    }
    return dp[0][0];
  }

  static int func3(vector<int> &d, vector<int> &p)
  {
    int sum = 0;
    for (int num : p)
    {
      sum += num;  // 贿赂所有怪兽需要的钱
    }
    // dp[i][j]含义：
    // 能经过0～i的怪兽，且花钱为j（花钱的严格等于j）时的武力值最大是多少？
    // 如果dp[i][j]==-1，表示经过0～i的怪兽，花钱为j是无法通过的，或者之前的钱怎么组合也得不到正>  好为j的钱数
    TdArray dp(d.size(), vector<int>(sum + 1));
    for (int i = 0; i < dp.size(); i++)
    {
      for (int j = 0; j <= sum; j++)
      {
        dp[i][j] = -1;
      }
    }
    // 经过0～i的怪兽，花钱数一定为p[0]，达到武力值d[0]的地步。其他第0行的状态一律是无效的
    dp[0][p[0]] = d[0];
    for (int i = 1; i < d.size(); i++)
    {
      for (int j = 0; j <= sum; j++)
      {
        // 可能性一，为当前怪兽花钱
        // 存在条件：
        // j - p[i]要不越界，并且在钱数为j - p[i]时，要能通过0～i-1的怪兽，并且钱数组合是有效的。
        if (j >= p[i] && dp[i - 1][j - p[i]] != -1)
        {
          dp[i][j] = dp[i - 1][j - p[i]] + d[i];
        }
        // 可能性二，不为当前怪兽花钱
        // 存在条件：
        // 0~i-1怪兽在花钱为j的情况下，能保证通过当前i位置的怪兽
        if (dp[i - 1][j] >= d[i])
        {
          // 两种可能性中，选武力值最大的
          dp[i][j] = std::max(dp[i][j], dp[i - 1][j]);
        }
      }
    }
    int ans = 0;
    // dp表最后一行上，dp[N-1][j]代表：
    // 能经过0～N-1的怪兽，且花钱为j（花钱的严格等于j）时的武力值最大是多少？
    // 那么最后一行上，最左侧的不为-1的列数(j)，就是答案
    for (int j = 0; j <= sum; j++)
    {
      if (dp[d.size() - 1][j] != -1)
      {
        ans = j;
        break;
      }
    }
    return ans;
  }

  static TdArray generateTwoRandomArray(int len, int value)
  {
    int size = getRandom(1, len);
    TdArray arrs(2, vector<int>(size));
    for (int i = 0; i < size; i++)
    {
      arrs[0][i] = getRandom(1, value);
      arrs[1][i] = getRandom(1, value);
    }
    return arrs;
  }

  static int getRandom(int min, int max)
  {
    random_device seed;                            // 硬件生成随机数种子
    ranlux48 engine(seed());                       // 利用种子生成随机数引
    uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
    int res = distrib(engine);                     // 随机数
    return res;
  }

  static void test()
  {
    int len = 10;
    int value = 20;
    int testTimes = 10000;
    cout << "测试开始" << endl;
    for (int i = 0; i < testTimes; i++)
    {
      TdArray arrs = generateTwoRandomArray(len, value);
      vector<int> d = arrs[0];
      vector<int> p = arrs[1];
      int ans1 = func1(d, p);
      int ans2 = func2(d, p);
      int ans3 = func3(d, p);
      int ans4 = minMoney2(d, p);
      if (ans1 != ans2 || ans2 != ans3 || ans1 != ans4)
      {
        cout << "oops!" << endl;
      }
    }
    cout << "测试结束" << endl;
  }
};

int main()
{
  MoneyProblem::test();
  return 0;
}
