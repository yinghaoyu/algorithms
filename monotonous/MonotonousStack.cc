#include <stdint.h>
#include <iostream>
#include <random>
#include <vector>

using namespace std;
using TdArray = vector<vector<int>>;

// 题目描述：给定一个不含有重复值的数组 arr，找到每一个 i 位置左边和右边离 i 位置最近且值比 arr[i]小的位置。返回所有位置相应的信息。

// arr = [ 3, 1, 2, 3]
//         0  1  2  3
//  [
//     0 : [-1,  1]
//     1 : [-1, -1]
//     2 : [ 1, -1]
//     3 : [ 2, -1]
//  ]

class MonotonousStack
{
 public:
  // 要求没有重复的数
  static TdArray getNearLessNoRepeat(vector<int> &arr)
  {
    TdArray res(arr.size(), vector<int>(2));
    // 只存位置！
    vector<int> stack;
    for (int i = 0; i < arr.size(); i++)
    {
      // 当遍历到i位置的数arr[i]，创建递增栈
      while (!stack.empty() && arr[stack.back()] > arr[i])
      {
        // 栈顶遇到更小的数arr[i]
        // 此时栈顶元素arr[j]左边最近的小值是栈顶的左边元素arr[leftLessIndex]
        // 栈顶右边的最近小值是arr[i]
        int j = stack.back();
        stack.pop_back();
        // 根据要求不存在，就返回-1
        int leftLessIndex = stack.empty() ? -1 : stack.back();
        res[j][0] = leftLessIndex;
        res[j][1] = i;
      }
      stack.push_back(i);
    }
    // 遍历所有元素后栈非空
    // 因为是单调递增栈，所以右边的小值不存在
    while (!stack.empty())
    {
      int j = stack.back();
      stack.pop_back();
      int leftLessIndex = stack.empty() ? -1 : stack.back();
      res[j][0] = leftLessIndex;
      res[j][1] = -1;
    }
    return res;
  }

  // 可以有重复的数
  static TdArray getNearLess(vector<int> &arr)
  {
    TdArray res(arr.size(), vector<int>(2));
    TdArray stack;
    for (int i = 0; i < arr.size(); i++)
    {
      // i -> arr[i] 进栈
      while (!stack.empty() && arr[stack.back().at(0)] > arr[i])
      {
        vector<int> popIs = stack.back();
        stack.pop_back();
        int leftLessIndex = stack.empty() ? -1 : stack.back().at(stack.back().size() - 1);
        for (int popi : popIs)  // 处理所有相同的数
        {
          res[popi][0] = leftLessIndex;
          res[popi][1] = i;
        }
      }
      // 把相同的数存在一个vector里面
      if (!stack.empty() && arr[stack.back().at(0)] == arr[i])
      {
        stack.back().push_back(i);
      }
      else  // 不同的话就新建一个vector
      {
        vector<int> list;
        list.push_back(i);
        stack.push_back(list);
      }
    }
    // 栈非空，此时是递增栈
    while (!stack.empty())
    {
      vector<int> popIs = stack.back();
      stack.pop_back();
      int leftLessIndex = stack.empty() ? -1 : stack.back().at(stack.back().size() - 1);
      for (int popi : popIs)
      {
        res[popi][0] = leftLessIndex;
        res[popi][1] = -1;
      }
    }
    return res;
  }

  // for test
  static vector<int> getRandomArrayNoRepeat(int size)
  {
    vector<int> arr(getRandom(0, size));
    for (int i = 0; i < arr.size(); i++)
    {
      arr[i] = i;
    }
    for (int i = 0; i < arr.size(); i++)
    {
      int swapIndex = getRandom(0, arr.size() - 1);
      int tmp = arr[swapIndex];
      arr[swapIndex] = arr[i];
      arr[i] = tmp;
    }
    return arr;
  }

  // for test
  static vector<int> getRandomArray(int size, int max)
  {
    vector<int> arr(getRandom(0, size));
    for (int i = 0; i < arr.size(); i++)
    {
      arr[i] = getRandom(0, max);
    }
    return arr;
  }

  // for test
  // 暴力解
  static TdArray rightWay(vector<int> &arr)
  {
    TdArray res(arr.size(), vector<int>(2));
    for (int i = 0; i < arr.size(); i++)
    {
      int leftLessIndex = -1;
      int rightLessIndex = -1;
      int cur = i - 1;
      while (cur >= 0)
      {
        if (arr[cur] < arr[i])
        {
          leftLessIndex = cur;
          break;
        }
        cur--;
      }
      cur = i + 1;
      while (cur < arr.size())
      {
        if (arr[cur] < arr[i])
        {
          rightLessIndex = cur;
          break;
        }
        cur++;
      }
      res[i][0] = leftLessIndex;
      res[i][1] = rightLessIndex;
    }
    return res;
  }

  // for test
  static bool isEqual(TdArray res1, TdArray res2)
  {
    if (res1.size() != res2.size())
    {
      return false;
    }
    for (int i = 0; i < res1.size(); i++)
    {
      if (res1[i][0] != res2[i][0] || res1[i][1] != res2[i][1])
      {
        return false;
      }
    }

    return true;
  }

  static int getRandom(int min, int max)
  {
    random_device seed;                            // 硬件生成随机数种子
    ranlux48 engine(seed());                       // 利用种子生成随机数引
    uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
    int res = distrib(engine);                     // 随机数
    return res;
  }

  // 为了测试
  static void printArray(vector<int> &arr)
  {
    for (int i = 0; i < arr.size(); i++)
    {
      cout << arr[i] << " ";
    }
    cout << endl;
  }

  static void test()
  {
    int size = 10;
    int max = 20;
    int testTimes = 2000000;

    cout << "测试开始" << endl;
    for (int i = 0; i < testTimes; i++)
    {
      vector<int> arr1 = getRandomArrayNoRepeat(size);
      vector<int> arr2 = getRandomArray(size, max);
      if (!isEqual(getNearLessNoRepeat(arr1), rightWay(arr1)))
      {
        cout << "No Repeat Oops!" << endl;
        printArray(arr1);
        break;
      }
      if (!isEqual(getNearLess(arr2), rightWay(arr2)))
      {
        cout << "Oops!" << endl;
        printArray(arr2);
        break;
      }
    }
    cout << "测试结束" << endl;
  }
};

int main()
{
  MonotonousStack::test();
  return 0;
}
