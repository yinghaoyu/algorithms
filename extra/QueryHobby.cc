#include <iostream>
#include <random>
#include <unordered_map>
#include <vector>

using namespace std;

// 问题描述：
// 数组为{3, 2, 2, 3, 1}，查询为(0, 3, 2)。意思是在数组里下标0~3这个范围上，有几个2？返回2。
// 假设给你一个数组arr，对这个数组的查询非常频繁，请返回所有查询的结果

class QueryHobby
{
 public:
  class QueryBox1
  {
   private:
    vector<int> arr;

   public:
    QueryBox1(vector<int> &array) { arr = array; }

    // 纯暴力解，时间复杂度O(N ^ 2)
    int query(int L, int R, int v)
    {
      int ans = 0;
      for (; L <= R; L++)
      {
        if (arr[L] == v)
        {
          ans++;
        }
      }
      return ans;
    }
  };

  class QueryBox2
  {
    // 预处理
   private:
    unordered_map<int, vector<int>> map;

   public:
    QueryBox2(vector<int> &arr)
    {
      for (int i = 0; i < arr.size(); i++)
      {
        if (map.find(arr[i]) == map.end())
        {
          map.emplace(arr[i], vector<int>(0));
        }
        map.at(arr[i]).push_back(i);
      }
    }

    int query(int L, int R, int value)
    {
      if (map.find(value) == map.end())
      {
        return 0;
      }
      vector<int> indexArr = map.at(value);
      // 查询 < L 的下标有几个
      int a = countLess(indexArr, L);
      // 查询 < R+1 的下标有几个
      int b = countLess(indexArr, R + 1);
      return b - a;
    }

    // 在有序数组arr中，用二分的方法数出<limit的数有几个
    // 也就是用二分法，找到<limit的数中最右的位置
    int countLess(vector<int> &arr, int limit)
    {
      int L = 0;
      int R = arr.size() - 1;
      int mostRight = -1;
      while (L <= R)
      {
        int mid = L + ((R - L) >> 1);
        if (arr.at(mid) < limit)
        {
          mostRight = mid;
          L = mid + 1;
        }
        else
        {
          R = mid - 1;
        }
      }
      return mostRight + 1;
    }
  };

  static vector<int> generateRandomArray(int len, int value)
  {
    vector<int> ans(getRandom(1, len));
    for (int i = 0; i < ans.size(); i++)
    {
      ans[i] = getRandom(1, value);
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
    int len = 300;
    int value = 20;
    int testTimes = 1000;
    int queryTimes = 1000;
    cout << "test begin" << endl;
    for (int i = 0; i < testTimes; i++)
    {
      vector<int> arr = generateRandomArray(len, value);
      int N = arr.size();
      QueryBox1 box1(arr);
      QueryBox2 box2(arr);
      for (int j = 0; j < queryTimes; j++)
      {
        int a = getRandom(0, N - 1);
        int b = getRandom(0, N - 1);
        int L = std::min(a, b);
        int R = std::max(a, b);
        int v = getRandom(1, value);
        if (box1.query(L, R, v) != box2.query(L, R, v))
        {
          cout << "Oops!" << endl;
        }
      }
    }
    cout << "test end" << endl;
  }
};

int main()
{
  QueryHobby::test();
  return 0;
}
