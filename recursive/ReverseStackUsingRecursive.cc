#include <iostream>
#include <vector>

using namespace std;

class ReverseStackUsingRecursive
{
 public:
  static void reverse(vector<int> &stack)
  {
    if (stack.empty())
    {
      return;
    }
    int i = f(stack);
    reverse(stack);
    stack.push_back(i);
  }

  // 栈底元素移除掉
  // 上面的元素盖下来
  // 返回移除掉的栈底元素
  static int f(vector<int> &stack)
  {
    int result = stack.back();
    stack.pop_back();
    if (stack.empty())
    {
      return result;
    }
    else
    {
      int last = f(stack);
      stack.push_back(result);
      return last;
    }
  }
};
int main()
{
  vector<int> test;
  test.push_back(1);
  test.push_back(2);
  test.push_back(3);
  test.push_back(4);
  test.push_back(5);
  ReverseStackUsingRecursive::reverse(test);
  while (!test.empty())
  {
    cout << test.back();
    test.pop_back();
  }
  cout << endl;
  return 0;
}
