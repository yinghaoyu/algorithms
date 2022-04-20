#include <iostream>
#include <string>
#include <string.h>
#include <random>

using namespace std;

int* getNextArray(const char* str2)
{
  int len = strlen(str2);
  if (len == 1)  // KMP默认第一个next数组元素为-1
  {
    return new int[1] { -1 };
  }
  int* next = new int[len];
  next[0] = -1;
  next[1] = 0;
  int i = 2; // 目前在哪个位置上求next数组的值
  int cn = 0; // 当前是哪个位置的值再和i-1位置的字符比较
  while (i < len)
  {
    if (str2[i - 1] == str2[cn])
    {
      // 配成功的时候
      next[i++] = ++cn;
    }
    else if (cn > 0)
    {
      cn = next[cn];
    }
    else
    {
      next[i++] = 0;
    }
  }
  return next;
}

// 返回s2在s1的下标
int getIndexOf(string s1, string s2)
{
  if(s2 == "" || s2.length() < 1)
  {
    return 0; // c++ 找空串默认找到下标为0
  }
  if (s1.length() < s2.length())
  {
    return -1;
  }
  const char* str1 = s1.c_str();
  const char* str2 = s2.c_str();
  int x = 0;
  int y = 0;
  // O(M) m <= n
  int* next = getNextArray(str2);
  // O(N)
  while (x < s1.length() && y < s2.length())
  {
    if (str1[x] == str2[y])
    {
      x++;
      y++;
    }
    else if (next[y] == -1)
    {
      // y == 0
      x++;
    }
    else
    {
      y = next[y];
    }
  }
  free(next);
  return y == s2.length() ? x - y : -1; // string::npos == -1
}

int getRandom(int min, int max)
{
  random_device seed;  // 硬件生成随机数种子
  ranlux48 engine(seed());  // 利用种子生成随机数引
  uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
  int res = distrib(engine);  // 随机数
  return res;
}

// for test
string getRandomString(int possibilities, int size)
{
  int len = getRandom(0, size);
  char* ans = new char[len + 1];
  for (int i = 0; i < len; i++)
  {
    int pos = getRandom(0, possibilities);
    ans[i] = (char) (pos + 'a');
  }
  ans[len] = '\0';
  return string(ans);
}

int main()
{
  int possibilities = 5;
  int strSize = 20;
  int matchSize = 5;
  int testTimes = 5000000;
  cout << "test begin" << endl;
  for (int i = 0; i < testTimes; i++)
  {
    string str = getRandomString(possibilities, strSize);
    string match = getRandomString(possibilities, matchSize);
    string a = "a";
    string b = "bbbbb";
    int c = getIndexOf(a, b);
    int d = a.find(b);
    if (getIndexOf(str, match) != str.find(match))
    {
      cout<< "str=" << str << " " << "match=" << match <<" "<<"Oops!" <<endl;
    }
  }
  cout << "test finish" <<endl;
  return 0;
}
