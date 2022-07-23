#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

class MinPeople
{
  // 问题描述：
  // 企鹅厂每年都会发文化衫，文化衫有很多种，厂庆的时候，企鹅们都需要穿文化衫来拍照
  // 一次采访中，记者随机遇到的企鹅，企鹅会告诉记者还有多少企鹅跟他穿一种文化衫
  // 我们将这些回答放在 answers 数组里，返回鹅厂中企鹅的最少数量。
  // 输入: answers = [1]    输出：2
  // 输入: answers = [1, 1, 2]    输出：5
  // Leetcode题目：https://leetcode.com/problems/rabbits-in-forest/
  //
  // 思想：先排序
  //  1 1 1 1 2 2 3 3 3 4
  //  0 1 2 3 4 5 6 7 8 9
  //  第0号人说，和我穿一样的还有1人
  //  第1号人说，和我穿一样的还有1人  -->让这两个人穿一样的
  //
  //  第2号人说，和我穿一样的还有1人
  //  第3号人说，和我穿一样的还有1人  -->让这两个人穿一样的
  //
  //  第6、7、8号人说，和我穿一样的还有3人，但是实际只统计了6、7、8三个人，说明有1个人没被统计
  //  采访中说出相同数量的人，最少需要的文化衫数 count(arr[i]) / (arr[i]+1)向上取整
  //  x / y向上取整，可严格表示为(x + y - 1) / y
 public:
  static int numRabbits(vector<int> &arr)
  {
    if (arr.size() == 0)
    {
      return 0;
    }
    std::sort(arr.begin(), arr.end());
    int x = arr[0];
    int c = 1;  // 统计相同值的个数
    int ans = 0;
    for (int i = 1; i < arr.size(); i++)
    {
      if (x != arr[i])
      {
        // c为报数量x的所有人
        // x + 1为每组的组内人员
        // c / (x + 1)向上取整，即为c个人至少分几组
        // c / (x + 1)向上取整，可严格表示为(c + x) / (x + 1)
        // 每组x + 1个人，因此要乘 x + 1，这里不能把(x+1)约掉 !!!
        ans += ((c + x) / (x + 1)) * (x + 1);
        x = arr[i];
        c = 1;
      }
      else
      {
        c++;
      }
    }
    return ans + ((c + x) / (x + 1)) * (x + 1);
  }
};

int main()
{
  return 0;
}
