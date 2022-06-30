#include <algorithm>
#include <iostream>
#include <random>
#include <set>
#include <string>
#include <vector>

using namespace std;

#define MAX_SEED 1024
#define HALF_SEED (MAX_SEED >> 1)

// 问题描述：
// 给定一个由字符串组成的数组，必须把所有的字符串拼接起来，返回所有可能的拼接结果中，字典序最小的结果。

class LowestDictionary
{
 public:
  class MyComparator
  {
   public:
    bool operator()(const string &o1, const string &o2) { return (o1 + o2) < (o2 + o1); }
  };

  typedef set<string, MyComparator> MySet;

  // 构造比较器
  static string lowestDictionary(vector<string> str)
  {
    if (str.size() < 1)
    {
      return "";
    }
    std::sort(str.begin(), str.end(), MyComparator());
    string result;
    for (string s : str)
    {
      result.append(s);
    }
    return result;
  }

  // 暴力对数器方法
  static string right(vector<string> strs)
  {
    if (strs.size() == 0)
    {
      return "";
    }
    MySet ans = process(strs);
    return ans.size() == 0 ? "" : *ans.begin();
  }

  // static string right2(vector<string> strs)
  // {
  //   if (strs.size() == 0)
  //   {
  //     return "";
  //   }
  //   MySet ans;
  //   do
  //   {
  //     string result;
  //     for (string s : strs)
  //     {
  //       result.append(s);
  //     }
  //     ans.emplace(result);
  //   } while (next_permutation(strs.begin(), strs.end())); // 使用前strs需先排序
  //   return ans.size() == 0 ? "" : *ans.begin();
  // }

  // strs中所有字符串全排列，返回所有可能的结果
  static MySet process(vector<string> strs)
  {
    MySet ans;
    if (strs.size() == 0)
    {
      ans.emplace("");
      return ans;
    }
    for (int i = 0; i < strs.size(); i++)
    {
      string first = strs[i];
      vector<string> nexts = removeIndexString(strs, i);
      MySet next = process(nexts);
      for (string cur : next)
      {
        ans.emplace(first + cur);
      }
    }
    return ans;
  }

  // 拷贝索引除外的字符串
  static vector<string> removeIndexString(vector<string> arr, int index)
  {
    int n = arr.size();
    vector<string> ans(n - 1);
    int ansIndex = 0;
    for (int i = 0; i < n; i++)
    {
      if (i != index)
      {
        ans[ansIndex++] = arr[i];
      }
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

  static void printArray(const vector<string> &arr)
  {
    for (string s : arr)
    {
      cout << s << endl;
    }
  }

  static void test()
  {
    int arrLen = 6;
    int strLen = 10;
    int testTimes = 10000;
    for (int i = 0; i < testTimes; i++)
    {
      vector<string> arr1 = generateRandomStringArray(arrLen, strLen);
      vector<string> arr2 = arr1;
      // printArray(arr1);
      if (lowestDictionary(arr1) != right(arr2))
      {
        for (string str : arr1)
        {
          cout << str + "," << endl;
        }
        cout << endl;
        cout << "Error!" << endl;
        break;
      }
    }
    cout << "finish!" << endl;
  }

  static vector<string> generateRandomStringArray(int arrLen, int strLen)
  {
    vector<string> ans(getRandom(1, arrLen));
    // ans.reverse(10) 只能预分配内存，不会调用构造函数
    for (int i = 0; i < ans.size(); i++)
    {
      ans[i] = generateRandomString(strLen);
    }
    return ans;
  }

  static string generateRandomString(int strLen)
  {
    string ans(getRandom(1, strLen), ' ');
    for (int i = 0; i < ans.size(); i++)
    {
      int value = getRandom(0, 5);
      ans[i] = (getRandom(0, MAX_SEED) <= HALF_SEED) ? (char) (65 + value) : (char) (97 + value);
    }
    return ans;
  }
};

int main()
{
  LowestDictionary::test();
  return 0;
}
