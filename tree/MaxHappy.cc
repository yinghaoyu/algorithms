#include <iostream>
#include <random>
#include <stdbool.h>
#include <vector>
#include <list>

using namespace std;

#define MAX_SEED 1024
#define HALF_SEED (MAX_SEED >> 1)
#define SMALL_SEED 16

// 派对最大快乐值

// 一个员工只有一个直接上级。也就是这个公司员工的层级结构就是一个多叉树。
// 现在公司邀请员工参加派对，要求不能同时邀请员工和员工的任一下级（即直接上下级不能同时邀请），如何邀请员工，才能使得参加派对的员工的快乐值是所有情况中最大的。最后返回最大的快乐值。

class MaxHappy
{
  public:
    class Employee
    {
      public:
        int happy;
        list<Employee*> nexts;

        Employee(int h)
        {
          happy = h;
        }
    };

    static int maxHappy1(Employee* boss)
    {
      if (boss == nullptr)
      {
        return 0;
      }
      return process1(boss, false);
    }

    // 当前来到的节点叫cur，
    // up表示cur的上级是否来，
    // 该函数含义：
    // 如果up为true，表示在cur上级已经确定来，的情况下，cur整棵树能够提供最大的快乐值是多少？
    // 如果up为false，表示在cur上级已经确定不来，的情况下，cur整棵树能够提供最大的快乐值是多少？
    static int process1(Employee* cur, bool up)
    {
      if (up)
      {
        // 如果cur的上级来的话，cur没得选，只能不来
        int ans = 0;
        for (Employee* next : cur->nexts)
        {
          ans += process1(next, false);
        }
        return ans;
      }
      else
      {
        // 如果cur的上级不来的话，cur可以选，可以来也可以不来
        int p1 = cur->happy;
        int p2 = 0;
        for (Employee* next : cur->nexts)
        {
          p1 += process1(next, true);
          p2 += process1(next, false);
        }
        return std::max(p1, p2);
      }
    }

    static int maxHappy2(Employee* head)
    {
      Info allInfo = process(head);
      return std::max(allInfo.no, allInfo.yes);
    }

    class Info
    {
      public:
        int no;
        int yes;

        Info(int n, int y)
        {
          no = n;
          yes = y;
        }
    };

    static Info process(Employee* x)
    {
      if (x == nullptr)
      {
        return Info(0, 0);
      }
      // x不来的最大快乐值
      int no = 0;
      // x来的最大快乐值
      int yes = x->happy;
      for (Employee* next : x->nexts)
      {
         // x不来，则求每个下级 来或不来 的最大值
        Info nextInfo = process(next);
        no += std::max(nextInfo.no, nextInfo.yes);
        // x来，则所有下级都不能来
        yes += nextInfo.no;
      }
      return Info(no, yes);
    }

    // for test
    static Employee* genarateBoss(int maxLevel, int maxNexts, int maxHappy)
    {
      if (getRandom(0, MAX_SEED) < SMALL_SEED)
      {
        return nullptr;
      }
      Employee* boss = new Employee(getRandom(0, maxHappy));
      genarateNexts(boss, 1, maxLevel, maxNexts, maxHappy);
      return boss;
    }

    // for test
    static void genarateNexts(Employee* e,
        int level,
        int maxLevel,
        int maxNexts,
        int maxHappy)
    {
      if (level > maxLevel)
      {
        return;
      }
      int nextsSize = getRandom(0, maxNexts);
      for (int i = 0; i < nextsSize; i++)
      {
        Employee* next = new Employee(getRandom(0, maxHappy));
        e->nexts.push_back(next);
        genarateNexts(next, level + 1, maxLevel, maxNexts, maxHappy);
      }
    }

    static int getRandom(int min, int max)
    {
      random_device seed;  // 硬件生成随机数种子
      ranlux48 engine(seed());  // 利用种子生成随机数引
      uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
      int res = distrib(engine);  // 随机数
      return res;
    }

    static void test()
    {
      int maxLevel = 4;
      int maxNexts = 7;
      int maxHappy = 100;
      int testTimes = 10000;
      for (int i = 0; i < testTimes; i++)
      {
        Employee* boss = genarateBoss(maxLevel, maxNexts, maxHappy);
        if (maxHappy1(boss) != maxHappy2(boss))
        {
          cout << "Oops!" << endl;
        }
      }
      cout << "finish!" << endl;
    }
};

int main()
{
  MaxHappy::test();
  return 0;
}
