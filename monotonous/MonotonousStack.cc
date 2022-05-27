#include <memory.h>
#include <stdbool.h>
#include <stdint.h>
#include <iostream>
#include <random>
#include <vector>

using namespace std;

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
  static int **getNearLessNoRepeat(int *arr, int len)
  {
    int **res = mallocArray(len, 2);
    // 只存位置！
    vector<int> stack;
    for (int i = 0; i < len; i++)
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
  static int **getNearLess(int *arr, int len)
  {
    int **res = mallocArray(len, 2);
    vector<vector<int>> stack;
    for (int i = 0; i < len; i++)
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
  static int *getRandomArrayNoRepeat(int size, int *len)
  {
    *len = getRandom(0, size);
    int *arr = new int[*len];
    for (int i = 0; i < *len; i++)
    {
      arr[i] = i;
    }
    for (int i = 0; i < *len; i++)
    {
      int swapIndex = getRandom(0, *len - 1);
      int tmp = arr[swapIndex];
      arr[swapIndex] = arr[i];
      arr[i] = tmp;
    }
    return arr;
  }

  // for test
  static int *getRandomArray(int size, int max, int *len)
  {
    *len = getRandom(0, size);
    int *arr = new int[*len];
    for (int i = 0; i < *len; i++)
    {
      arr[i] = getRandom(0, max);
    }
    return arr;
  }

  // for test
  // 暴力解
  static int **rightWay(int *arr, int len)
  {
    int **res = mallocArray(len, 2);
    for (int i = 0; i < len; i++)
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
      while (cur < len)
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
  static bool isEqual(int **res1, int **res2, int len1, int len2)
  {
    if (len1 != len2)
    {
      return false;
    }
    for (int i = 0; i < len1; i++)
    {
      if (res1[i][0] != res2[i][0] || res1[i][1] != res2[i][1])
      {
        return false;
      }
    }

    return true;
  }

  // for test
  static int **mallocArray(int row, int column)
  {
    int **arr = (int **) malloc(sizeof(int *) * row);
    for (int i = 0; i < row; i++)
    {
      arr[i] = (int *) malloc(sizeof(int) * column);
      memset(arr[i], 0, sizeof(int) * column);
    }
    return arr;
  }

  static void freeArray(int **arr, int row)
  {
    for (int i = 0; i < row; i++)
    {
      free(arr[i]);
    }
    free(arr);
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
  static void printArray(int *arr, int len)
  {
    for (int i = 0; i < len; i++)
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
    int len1 = 0;
    int len2 = 0;

    cout << "测试开始" << endl;
    for (int i = 0; i < testTimes; i++)
    {
      int *arr1 = getRandomArrayNoRepeat(size, &len1);
      int *arr2 = getRandomArray(size, max, &len2);
      if (!isEqual(getNearLessNoRepeat(arr1, len1), rightWay(arr1, len1), len1, len1))
      {
        cout << "No Repeat Oops!" << endl;
        printArray(arr1, len1);
        break;
      }
      if (!isEqual(getNearLess(arr2, len2), rightWay(arr2, len2), len2, len2))
      {
        cout << "Oops!" << endl;
        printArray(arr2, len2);
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
