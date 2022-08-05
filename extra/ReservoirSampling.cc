#include <iostream>
#include <random>
#include <vector>

using namespace std;

// 问题描述：给定一个数据流，数据流长度N很大，且N直到处理完所有数据之前都不可知，请问如何在只遍历一遍数据（O(N)）的情况下，能够随机选取出m个不重复的数据。
//
// 这个场景强调了3件事：
// 1、数据流长度N很长且不可知，所以不能一次性装入内存。
// 2、时间复杂度为严格的O(N)。
// 3、随机抽取m个数，每个数被选中的概率为 m / N 。
// 第1点限制了不能取N内的m个随机数，然后按索引取出数据。
// 第2点限制了不能先遍历一遍，然后分块存储数据，再随机抽取。
// 第3点是保证数据选取绝对随机。
//
// 蓄水池抽样算法
// 1、如果接收的数据量小于m，则依次放入蓄水池。
// 2、当接收到第i个数据时，i >= m，在[0, i]范围内取以随机数d，
//    若d的落在[0, m-1]范围内，则用接收到的第i个数据替换蓄水池中的第d个数据。
// 3、重复步骤2。

// 证明：
// 第i个接收到的数据最后能够留在蓄水池中的概率=第i个数据进入过蓄水池的概率*之后第i个数据不被替换的概率(第i+1到第N次处理数据都不会被替换）

// 1、当i <= m，第i个数据进入过蓄水池的概率 = 1。
// 2、当i > m，在[1, i]内选取d，如果d <= m，
//    则使用第i个数据替换蓄水池中第d个数据，因此第i个数据进入蓄水池的概率 = m / i。
// 3、当i <= m，蓄水池从接收到第m+1个数据开始执行替换，
//    ① 第m+1次会替换池中数据概率为m/(m+1)，会替换掉第i个数据的概率为1/m，
//    则第m+1次处理替换掉第i个数据的概率为(m/(m+1))*(1/m)=1/(m+1)，
//    不被替换的概率为1-1/(m+1)=m/(m+1)。
//    ② 第m+2次会替换池中数据概率为m/(m+2)，会替换掉第i个数据的概率为1/m，
//    则第m+2次处理替换掉第i个数据的概率为(m/(m+2))*(1/m)=1/(m+2)，
//    不被替换的概率为1-1/(m+2)=(m+1)/(m+2)。
//    ③ ...
//    ④ 第N次处理不替换掉第i个数据的概率为(N-1)/N。
//    所以，之后第i个数据不被替换的概率=m/(m+1)*(m+1)/(m+2)*...*(N-1)/N= m / N。
// 4、当i > m，蓄水池从接收到第i+1个数据时开始有可能替换第i个数据。
//    则参考上述第3点，之后第i个数据不被替换的概率 = i / N。
// 5、结合第1点和第3点可知，当i<=m时，第i个接收到的数据最后留在蓄水池中的概率=1*m/N= m / N。
//    结合第2点和第4点可知，当i>m时，第i个接收到的数据留在蓄水池中的概率=m/i*i/N= m / N。
//    综上，每个数据最后被选中留在蓄水池中的概率为m / N。

class ReservoirSampling
{
 public:
  class RandomBox
  {
   public:
    vector<int> bag;
    int N;      // 蓄水池的容量m
    int count;  // 当前接收的数据序号i

    RandomBox(int capacity)
    {
      bag = vector<int>(capacity);
      N = capacity;
      count = 0;
    }

    int rand(int max) { return getRandom(1, max); }

    void add(int num)
    {
      count++;
      if (count <= N)
      {
        // 小于蓄水池的容量正常加入
        bag[count - 1] = num;
      }
      else
      {
        // 大于蓄水池的容量
        // 在[1, count]取随机数 r 如果1 <= r <= N，就替换bag[r-1]的为num
        // 这里用了两次随机，不影响
        if (rand(count) <= N)
        {
          bag[rand(N) - 1] = num;
        }
      }
    }

    vector<int> choices()
    {
      vector<int> ans(N);
      for (int i = 0; i < N; i++)
      {
        ans[i] = bag[i];
      }
      return ans;
    }
  };

  // 请等概率返回1~i中的一个数字
  static int random(int i) { return getRandom(1, i); }

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
    cout << "hello" << endl;
    int all = 100;
    int choose = 10;
    int testTimes = 50000;
    vector<int> counts(all + 1);
    for (int i = 0; i < testTimes; i++)
    {
      RandomBox box(choose);
      for (int num = 1; num <= all; num++)
      {
        box.add(num);
      }
      vector<int> ans = box.choices();
      for (int j = 0; j < box.N; j++)
      {
        counts[ans[j]]++;
      }
    }

    for (int i = 0; i < all + 1; i++)
    {
      cout << i << " times : " << counts[i] << endl;
    }
  }
};

int main()
{
  ReservoirSampling::test();
}
