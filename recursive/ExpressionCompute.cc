#include <deque>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

// (1+2)*3+4*(5+6)/7-8
// f(1) 返回 {3, 4}表示1..4计算结果为3
//           f(11) 返回 {11, 14}表示11..14计算结果为11
//
// 测试链接 : https://leetcode.com/problems/basic-calculator-iii/

class ExpressionCompute
{
 public:
  static int calculate(string &str) { return f(str, 0)[0]; }

  // 请从str[i...]往下算，遇到字符串终止位置或者右括号，就停止
  // 返回两个值，长度为2的数组
  // 0) 负责的这一段的结果是多少
  // 1) 负责的这一段计算到了哪个位置
  static vector<int> f(string &str, int i)
  {
    deque<string> que;
    int cur = 0;
    vector<int> bra;
    // 从i出发，开始撸串
    while (i < str.length() && str[i] != ')')
    {
      if (str[i] >= '0' && str[i] <= '9')
      {
        cur = cur * 10 + str[i++] - '0';
      }
      else if (str[i] != '(')
      {
        // 遇到的是运算符号
        addNum(que, cur);
        que.push_back(string(1, str[i++]));
        cur = 0;  // 这里要把cur清0，重新计算下一个数
      }
      else
      {  // 遇到左括号了
        bra = f(str, i + 1);
        cur = bra[0];
        i = bra[1] + 1;
      }
    }
    addNum(que, cur);
    return vector<int>{getNum(que), i};
  }

  // 入队前，先把乘法、除法先计算
  static void addNum(deque<string> &que, int num)
  {
    if (!que.empty())
    {
      int cur = 0;
      string top = que.back();
      que.pop_back();
      if (top == "+" || top == "-")  // 如果是加法减法就无事发生
      {
        que.push_back(top);
      }
      else  // 如果是乘法除法，则先计算结果，再把结果入队
      {
        cur = atoi(que.back().c_str());
        que.pop_back();
        num = top == "*" ? (cur * num) : (cur / num);
      }
    }
    que.push_back(std::to_string(num));
  }

  // 队列只剩加法、减法了
  static int getNum(deque<string> &que)
  {
    int res = 0;
    bool add = true;  // 如果没有负号，默认是正数
    string cur;
    int num = 0;
    while (!que.empty())
    {
      cur = que.front();
      que.pop_front();
      if (cur == "+")
      {
        add = true;
      }
      else if (cur == "-")
      {
        add = false;
      }
      else
      {
        num = atoi(cur.c_str());
        res += add ? num : (-num);
      }
    }
    return res;
  }
};

int main()
{
  string exp = "(1+2)*3+4*(5+6)/7-8";
  string ans = "7";
  cout << exp << "=" << ans << endl;
  cout << exp << "=" << ExpressionCompute::calculate(exp) << endl;
  return 0;
}
