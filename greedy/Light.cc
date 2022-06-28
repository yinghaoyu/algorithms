#include <iostream>
#include <random>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

#define MAX_SEED 1024
#define HALF_SEED (MAX_SEED >> 1)

// 问题描述：
// 给定一个字符串str，只由‘X’和‘.’两种字符构成。
// ‘X’表示墙，不能放灯，也不需要点亮。
// ‘.’表示居民点，可以放灯，需要点亮。
// 如果灯放在i位置，可以让i-1， i和i+1三个位置被点亮。
// 返回如果点亮str中所有需要点亮的位置，至少需要几盏灯。

class Light
{
 public:
  static int minLight1(string road)
  {
    if (road.length() == 0)
    {
      return 0;
    }
    unordered_set<int> lights;
    return process(road, 0, lights);
  }

  // str[index....]位置，自由选择放灯还是不放灯
  // str[0..index-1]位置呢？已经做完决定了，那些放了灯的位置，存在lights里
  // 要求选出能照亮所有.的方案，并且在这些有效的方案中，返回最少需要几个灯
  static int process(string str, int index, unordered_set<int> &lights)
  {
    if (index == str.length())
    {
      // 结束的时候
      for (int i = 0; i < str.length(); i++)
      {
        if (str[i] == '.')
        {
          // 当前位置是点的话
          if (lights.find(i - 1) == lights.end() && lights.find(i) == lights.end() && lights.find(i + 1) == lights.end())
          {
            // 只要有一处没被点亮，说明不满足条件
            return INT32_MAX;
          }
        }
      }
      return lights.size();
    }
    else
    {
      // str还没结束
      // i X .
      int no = process(str, index + 1, lights);  // 不点灯后续
      int yes = INT32_MAX;
      if (str[index] == '.')  // 这种情况才能点灯
      {
        lights.emplace(index);                  // 点灯后加入集合
        yes = process(str, index + 1, lights);  // 点灯后续
        lights.erase(index);                    // 恢复
      }
      return std::min(no, yes);
    }
  }

  static int minLight2(string road)
  {
    int i = 0;
    int light = 0;
    while (i < road.length())
    {
      if (road[i] == 'X')
      {
        i++;
      }
      else
      {
        light++;
        if (i + 1 == road.length())
        {
          // 到达边界
          break;
        }
        else
        {
          // 有i位置  i+ 1   X  .
          if (road[i + 1] == 'X')
          {
            // i处的灯光被i+1的墙挡住了
            i = i + 2;
          }
          else
          {
            // i+1不是墙，由于是从左往右遍历，i处的灯，可以移到i+1处
            // i+1可以影响i和i+2
            i = i + 3;
          }
        }
      }
    }
    return light;
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
  static string randomString(int len)
  {
    string res(getRandom(1, len), 'X');
    for (int i = 0; i < res.length(); i++)
    {
      res[i] = getRandom(0, MAX_SEED) < HALF_SEED ? 'X' : '.';
    }
    return res;
  }

  static void test()
  {
    int len = 20;
    int testTime = 100000;
    for (int i = 0; i < testTime; i++)
    {
      string test = randomString(len);
      int ans1 = minLight1(test);
      int ans2 = minLight2(test);
      if (ans1 != ans2)
      {
        cout << "oops!" << endl;
      }
    }
    cout << "finish!" << endl;
  }
};

int main()
{
  Light::test();
  return 0;
}
