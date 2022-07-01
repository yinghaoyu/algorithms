#include <iostream>
#include <list>

using namespace std;

// 卡特兰数组

class Catalan
{
 public:
  static long ways1(int N)
  {
    int zero = N;
    int one = N;
    list<int> path;
    list<list<int>> ans;
    process(zero, one, path, ans);
    long count = 0;
    for (list<int> cur : ans)
    {
      int status = 0;
      for (int num : cur)
      {
        if (num == 0)
        {
          status++;
        }
        else
        {
          status--;
        }
        if (status < 0)
        {
          break;
        }
      }
      if (status == 0)
      {
        count++;
      }
    }
    return count;
  }

  static void process(int zero, int one, list<int> &path, list<list<int>> &ans)
  {
    if (zero == 0 && one == 0)
    {
      list<int> cur;
      for (int num : path)
      {
        cur.push_back(num);
      }
      ans.push_back(cur);
    }
    else
    {
      if (zero == 0)
      {
        path.push_back(1);
        process(zero, one - 1, path, ans);
        path.pop_back();
      }
      else if (one == 0)
      {
        path.push_back(0);
        process(zero - 1, one, path, ans);
        path.pop_back();
      }
      else
      {
        path.push_back(1);
        process(zero, one - 1, path, ans);
        path.pop_back();
        path.push_back(0);
        process(zero - 1, one, path, ans);
        path.pop_back();
      }
    }
  }

  static long ways2(int N)
  {
    if (N < 0)
    {
      return 0;
    }
    if (N < 2)
    {
      return 1;
    }
    long a = 1;
    long b = 1;
    long limit = N << 1;
    for (long i = 1; i <= limit; i++)
    {
      if (i <= N)
      {
        a *= i;
      }
      else
      {
        b *= i;
      }
    }
    return (b / a) / (N + 1);
  }

  static void test()
  {
    cout << "test begin" << endl;
    for (int i = 0; i < 10; i++)
    {
      long ans1 = ways1(i);
      long ans2 = ways2(i);
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
  Catalan::test();
  return 0;
}
