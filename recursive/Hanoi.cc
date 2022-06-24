#include <stdbool.h>
#include <iostream>
#include <vector>

using namespace std;

// 汉诺塔问题：有三根杆(编号A、B、C)，在A杆自下而上、由大到小按顺序放置金盘。
// 游戏的目标：把A杆上的金盘全部移到C杆上，并仍保持原有顺序叠好。
// 操作规则：每次只能移动一个盘子，并且在移动过程中三根杆上都始终保持大盘在下，小盘在上，操作过程中盘子可以置于A、B、C任一杆上。

class Hanoi
{
 public:
  // 请把1~N层圆盘 从左 -> 右
  static void leftToRight(int n)
  {
    if (n == 1)
    {
      // base case
      cout << "Move 1 from left to right" << endl;
      return;
    }
    leftToMid(n - 1);
    cout << "Move " << n << " from left to right" << endl;
    midToRight(n - 1);
  }

  // 请把1~N层圆盘 从左 -> 中
  static void leftToMid(int n)
  {
    if (n == 1)
    {
      cout << "Move 1 from left to mid" << endl;
      return;
    }
    leftToRight(n - 1);
    cout << "Move " << n << " from left to mid" << endl;
    rightToMid(n - 1);
  }

  static void rightToMid(int n)
  {
    if (n == 1)
    {
      cout << "Move 1 from right to mid" << endl;
      return;
    }
    rightToLeft(n - 1);
    cout << "Move " << n << " from right to mid" << endl;
    leftToMid(n - 1);
  }

  static void midToRight(int n)
  {
    if (n == 1)
    {
      cout << "Move 1 from mid to right" << endl;
      return;
    }
    midToLeft(n - 1);
    cout << "Move " << n << " from mid to right" << endl;
    leftToRight(n - 1);
  }

  static void midToLeft(int n)
  {
    if (n == 1)
    {
      cout << "Move 1 from mid to left" << endl;
      return;
    }
    midToRight(n - 1);
    cout << "Move " << n << " from mid to left" << endl;
    rightToLeft(n - 1);
  }

  static void rightToLeft(int n)
  {
    if (n == 1)
    {
      cout << "Move 1 from right to left" << endl;
      return;
    }
    rightToMid(n - 1);
    cout << "Move " << n << " from right to left" << endl;
    midToLeft(n - 1);
  }

  /******************** Way 1 *****************************/
  static void hanoi1(int n) { leftToRight(n); }

  /******************** Way 2 *****************************/
  static void hanoi2(int n)
  {
    if (n > 0)
    {
      func(n, "left", "right", "mid");
    }
  }

  static void func(int N, string from, string to, string other)
  {
    if (N == 1)
    {  // base
      cout << "Move 1 from " + from + " to " + to << endl;
    }
    else
    {
      func(N - 1, from, other, to);
      cout << "Move " << N << " from " << from << " to " << to << endl;
      func(N - 1, other, to, from);
    }
  }

  class Record
  {
   public:
    bool finish1;
    int base;
    string from;
    string to;
    string other;

    Record(bool f1, int b, string f, string t, string o)
    {
      finish1 = false;
      base = b;
      from = f;
      to = t;
      other = o;
    }
  };

  /******************** Way 3 *****************************/
  static void hanoi3(int N)
  {
    if (N < 1)
    {
      return;
    }
    vector<Record> stack;
    stack.emplace_back(Record(false, N, "left", "right", "mid"));
    while (!stack.empty())
    {
      Record cur = stack.back();
      stack.pop_back();
      if (cur.base == 1)
      {
        cout << "Move 1 from " << cur.from << " to " << cur.to << endl;
        if (!stack.empty())
        {
          stack.back().finish1 = true;
        }
      }
      else
      {
        if (!cur.finish1)
        {
          stack.push_back(cur);
          stack.emplace_back(Record(false, cur.base - 1, cur.from, cur.other, cur.to));
        }
        else
        {
          cout << "Move " << cur.base << " from " << cur.from << " to " << cur.to << endl;
          stack.emplace_back(Record(false, cur.base - 1, cur.other, cur.to, cur.from));
        }
      }
    }
  }
};

int main()
{
  int n = 3;
  Hanoi::hanoi1(n);
  cout << "============" << endl;
  Hanoi::hanoi2(n);
  cout << "============" << endl;
  Hanoi::hanoi3(n);
  return 0;
}
