#include <algorithm>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

// 问题描述：
// 一些会议要占用一个会议室宣讲，会议室不能同时容纳两个会议的宣讲。
// 给你每个项目的开始时间和结束时间，你来安排宣讲的日程，要求会议室进行宣讲的场次最多。
// 返回最多的宣讲场次数量。

class BestArrange
{
 public:
  class Program
  {
   public:
    int start;
    int end;

    Program()  // 为了兼容vector<Program>
    {
      start = 0;
      end = 0;
    }

    Program(int start, int end)
    {
      this->start = start;
      this->end = end;
    }

    bool operator<(const Program &other) { return this->end < other.end; }
  };

  // 暴力！所有情况都尝试！
  static int bestArrange1(vector<Program> &programs)
  {
    if (programs.size() == 0)
    {
      return 0;
    }
    return process(programs, 0, 0);
  }

  // 还剩下的会议都放在programs里
  // done之前已经安排了多少会议的数量
  // timeLine目前来到的时间点是什么
  // 目前来到timeLine的时间点，已经安排了done多的会议，剩下的会议programs可以自由安排
  // 返回能安排的最多会议数量
  static int process(vector<Program> &programs, int done, int timeLine)
  {
    if (programs.size() == 0)
    {
      return done;
    }
    // 还剩下会议
    int max = done;
    // 当前安排的会议是什么会，每一个都枚举
    for (int i = 0; i < programs.size(); i++)
    {
      if (programs[i].start >= timeLine)  // 只有开始时间比时间线晚的会议才能被安排
      {
        vector<Program> next = copyButExcept(programs, i);
        max = std::max(max, process(next, done + 1, programs[i].end));
      }
    }
    return max;
  }

  static vector<Program> copyButExcept(vector<Program> programs, int i)
  {
    vector<Program> ans(programs.size() - 1);
    int index = 0;
    for (int k = 0; k < programs.size(); k++)
    {
      if (k != i)
      {
        ans[index++] = programs[k];
      }
    }
    return ans;
  }

  // 会议的开始时间和结束时间，都是数值，不会 < 0
  static int bestArrange2(vector<Program> &programs)
  {
    std::sort(programs.begin(), programs.end());  // 把会议按结束时间进行排序
    int timeLine = 0;
    int result = 0;
    // 依次遍历每一个会议，结束时间早的会议先遍历
    for (int i = 0; i < programs.size(); i++)
    {
      if (timeLine <= programs[i].start)
      {
        result++;
        timeLine = programs[i].end;
      }
    }
    return result;
  }

  // for test
  static vector<Program> generatePrograms(int programSize, int timeMax)
  {
    vector<Program> ans(getRandom(0, programSize));
    for (int i = 0; i < ans.size(); i++)
    {
      int r1 = getRandom(0, timeMax);
      int r2 = getRandom(0, timeMax);
      if (r1 == r2)
      {
        ans[i] = Program(r1, r1 + 1);
      }
      else
      {
        ans[i] = Program(std::min(r1, r2), std::max(r1, r2));
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

  static void test()
  {
    int programSize = 12;
    int timeMax = 20;
    int timeTimes = 1000000;
    for (int i = 0; i < timeTimes; i++)
    {
      vector<Program> programs = generatePrograms(programSize, timeMax);
      if (bestArrange1(programs) != bestArrange2(programs))
      {
        cout << "Oops!" << endl;
      }
    }
    cout << "finish!" << endl;
  }
};

int main()
{
  BestArrange::test();
  return 0;
}
