#include <cstdint>
#include <vector>

using namespace std;

// 用递归函数排序栈
// 栈只提供push、pop、isEmpty三个方法
// 请完成无序栈的排序，要求排完序之后，从栈顶到栈底从小到大
// 只能使用栈提供的push、pop、isEmpty三个方法、以及递归函数
// 除此之外不能使用任何的容器，数组也不行
// 就是排序过程中只能用：
// 1) 栈提供的push、pop、isEmpty三个方法
// 2) 递归函数，并且返回值最多为单个整数
class SortvectorWithRecursive
{
 public:
  void sort(vector<int>& stack)
  {
    int deep = Deep(stack);
    while (deep > 0)
    {
      int max = Max(stack, deep);
      int k = Times(stack, deep, max);
      down(stack, deep, max, k);
      deep -= k;
    }
  }

  // 返回栈的深度
  // 不改变栈的数据状况
  int Deep(vector<int>& stack)
  {
    if (stack.empty())
    {
      return 0;
    }
    int num = stack.back();
    stack.pop_back();
    int deep = Deep(stack) + 1;
    stack.push_back(num);
    return deep;
  }

  // 从栈当前的顶部开始，往下数deep层
  // 返回这deep层里的最大值
  int Max(vector<int>& stack, int deep)
  {
    if (deep == 0)
    {
      return INT32_MIN;
    }
    int num = stack.back();
    stack.pop_back();
    int restMax = Max(stack, deep - 1);
    int max = std::max(num, restMax);
    stack.push_back(num);
    return max;
  }

  // 从栈当前的顶部开始，往下数deep层，已知最大值是max了
  // 返回，max出现了几次，不改变栈的数据状况
  int Times(vector<int> stack, int deep, int max)
  {
    if (deep == 0)
    {
      return 0;
    }
    int num = stack.back();
    stack.pop_back();
    int restTimes = Times(stack, deep - 1, max);
    int times = restTimes + (num == max ? 1 : 0);
    stack.push_back(num);
    return times;
  }

  // 从栈当前的顶部开始，往下数deep层，已知最大值是max，出现了k次
  // 请把这k个最大值沉底，剩下的数据状况不变
  void down(vector<int> stack, int deep, int max, int k)
  {
    if (deep == 0)
    {
      for (int i = 0; i < k; i++)
      {
        stack.push_back(max);
      }
    }
    else
    {
      int num = stack.back();
      stack.pop_back();
      down(stack, deep - 1, max, k);
      if (num != max)
      {
        stack.push_back(num);
      }
    }
  }
};
