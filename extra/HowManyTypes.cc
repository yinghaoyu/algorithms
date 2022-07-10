#include <iostream>
#include <random>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

// 问题描述：
// 只由小写字母（a~z）组成的一批字符串，都放在字符类型的数组String[] arr中，
// 如果其中某两个字符串，所含有的字符种类完全一样，就将两个字符串算作一类
// 比如：baacba和bac就算作一类
// 虽然长度不一样，但是所含字符的种类完全一样（a、b、c） 返回arr中有多少类？

class HowManyTypes
{
 public:
  static int types1(vector<string> &arr)
  {
    unordered_set<string> types;
    for (string str : arr)
    {
      vector<bool> map(26);
      for (int i = 0; i < str.length(); i++)
      {
        // 统字符出现的次数
        map[str[i] - 'a'] = true;
      }
      string key = "";
      for (int i = 0; i < 26; i++)
      {
        if (map[i])
        {
          // 统计该字符类
          key += i + 'a';
        }
      }
      // hash 去重
      types.emplace(key);
    }
    return types.size();
  }

  // 位运算优化
  static int types2(vector<string> &arr)
  {
    unordered_set<int> types;
    for (string str : arr)
    {
      int key = 0;
      for (int i = 0; i < str.length(); i++)
      {
        key |= (1 << (str[i] - 'a'));
      }
      types.emplace(key);
    }
    return types.size();
  }

  // for test
  static vector<string> getRandomStringArray(int possibilities, int strMaxSize, int arrMaxSize)
  {
    vector<string> ans(getRandom(1, arrMaxSize));
    for (int i = 0; i < ans.size(); i++)
    {
      ans[i] = getRandomString(possibilities, strMaxSize);
    }
    return ans;
  }

  // for test
  static string getRandomString(int possibilities, int strMaxSize)
  {
    string ans(getRandom(1, strMaxSize), ' ');
    for (int i = 0; i < ans.length(); i++)
    {
      ans[i] = getRandom(0, possibilities - 1) + 'a';
    }
    return ans;
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
    int possibilities = 5;
    int strMaxSize = 10;
    int arrMaxSize = 100;
    int testTimes = 500000;
    cout << "test begin, test time : " << testTimes << endl;
    for (int i = 0; i < testTimes; i++)
    {
      vector<string> arr = getRandomStringArray(possibilities, strMaxSize, arrMaxSize);
      int ans1 = types1(arr);
      int ans2 = types2(arr);
      if (ans1 != ans2)
      {
        cout << "Oops!" << endl;
      }
    }
    cout << "test finish" << endl;
  }
};

int main()
{
  HowManyTypes::test();
  return 0;
}
