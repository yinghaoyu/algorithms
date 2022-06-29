#include <iostream>
#include <queue>
#include <random>

using namespace std;

// 问题描述：
// 一块金条切成两半，是需要花费和长度数值一样的铜板的。
// 比如长度为20的金条，不管怎么切，都要花费20个铜板。
// 一群人想整分整块金条，怎么分最省铜板？输入一个数组，返回分割的最小代价。
// 例如：
// 给定数组{10,20,30}，代表一共三个人， 整块金条长度为10 + 20 + 30 = 60，金条要分成10, 20, 30三个部分（不考虑顺序）。
// 如果先把长度60的金条分成10和50，花费60；
// 再把长度50的金条分成20和30，花费50;一共花费110铜板。
// 但如果先把长度60的金条分成30和30，花费60；
// 再把长度30金条分成10和20，花费30；一共花费90铜板。

class LessMoneySplitGold
{
 public:
  // 纯暴力
  static int lessMoney1(vector<int> &arr)
  {
    if (arr.size() == 0)
    {
      return 0;
    }
    return process(arr, 0);
  }

  // 等待合并的数都在arr里，pre之前的合并行为产生了多少总代价
  // arr中只剩一个数字的时候，停止合并，返回最小的总代价
  static int process(vector<int> arr, int pre)
  {
    if (arr.size() == 1)
    {
      return pre;
    }
    int ans = INT32_MAX;
    for (int i = 0; i < arr.size(); i++)
    {
      for (int j = i + 1; j < arr.size(); j++)
      {
        ans = std::min(ans, process(copyAndMergeTwo(arr, i, j), pre + arr[i] + arr[j]));
      }
    }
    return ans;
  }

  // 把arr中arr[i]和arr[j]两个元素合并
  static vector<int> copyAndMergeTwo(vector<int> arr, int i, int j)
  {
    vector<int> ans(arr.size() - 1);
    int ansi = 0;
    for (int arri = 0; arri < arr.size(); arri++)
    {
      if (arri != i && arri != j)
      {
        ans[ansi++] = arr[arri];
      }
    }
    ans[ansi] = arr[i] + arr[j];
    return ans;
  }

  static int lessMoney2(vector<int> &arr)
  {
    // priority_queue<int> pQ; // 默认是大根堆
    priority_queue<int, vector<int>, greater<int>> pQ;  // 小根堆
    for (int i = 0; i < arr.size(); i++)
    {
      pQ.emplace(arr[i]);
    }
    int sum = 0;
    int cur = 0;
    while (pQ.size() > 1)
    {
      int v1 = pQ.top();
      pQ.pop();
      int v2 = pQ.top();
      pQ.pop();
      cur = v1 + v2;
      sum += cur;
      pQ.emplace(cur);
    }
    return sum;
  }

  static int getRandom(int min, int max)
  {
    random_device seed;                            // 硬件生成随机数种子
    ranlux48 engine(seed());                       // 利用种子生成随机数引
    uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
    int res = distrib(engine);                     // 随机数
    return res;
  }

  // for test
  static vector<int> generateRandomArray(int maxSize, int maxValue)
  {
    vector<int> arr(getRandom(0, maxSize));
    for (int i = 0; i < arr.size(); i++)
    {
      arr[i] = getRandom(0, maxValue);
    }
    return arr;
  }

  static void test()
  {
    int testTime = 100000;
    int maxSize = 6;
    int maxValue = 1000;
    for (int i = 0; i < testTime; i++)
    {
      vector<int> arr = generateRandomArray(maxSize, maxValue);
      if (lessMoney1(arr) != lessMoney2(arr))
      {
        cout << "Oops!" << endl;
      }
    }
    cout << "finish!" << endl;
  }
};

int main()
{
  LessMoneySplitGold::test();
  return 0;
}
