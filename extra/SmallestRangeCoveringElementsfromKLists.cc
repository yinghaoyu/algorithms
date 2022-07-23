#include <iostream>
#include <set>
#include <vector>

using namespace std;
using TdArray = vector<vector<int>>;

// 问题描述：
// 你有 k 个 非递减排列 的整数列表。
// 找到一个 最小 区间，使得 k 个列表中的每个列表至少有一个数包含在其中。
// 我们定义如果 b-a < d-c 或者在 b-a == d-c 时 a < c，则区间 [a,b] 比 [c,d] 小。

// 测试链接 : https://leetcode.com/problems/smallest-range-covering-elements-from-k-lists/
class SmallestRangeCoveringElementsfromKLists
{
 public:
  class Node
  {
   public:
    int value;  // 数组的值
    int arrid;  // 数组所在的行号
    int index;  // 值在第几列

    Node(int v, int ai, int i)
    {
      value = v;
      arrid = ai;
      index = i;
    }
  };

  class Comparator
  {
   public:
    // 为了防止两个相同value的Node覆盖，将地址纳入比较规则，确保可以有相同value的Node
    bool operator()(const Node &o1, const Node &o2) { return o1.value != o2.value ? o1.value - o2.value : &o1 - &o2; }
  };

  static vector<int> smallestRange(TdArray &nums)
  {
    int N = nums.size();
    set<Node, Comparator> orderSet;
    // nums = {
    //  {1, 5, 6},
    //  {2, 7, 9},
    //  {4, 6, 8, 12}
    //}
    for (int i = 0; i < N; i++)
    {
      // 先把每行的第0列加入序列
      // 因为每行都可能有重复的值，而set不能加入重复的值，因此封装成Node
      orderSet.emplace(Node(nums[i][0], i, 0));
    }
    bool set = false;
    int a = 0;
    int b = 0;
    // 每一行的数组元素不一定相等
    // 保证每行的数组都至少有一个数
    while (orderSet.size() == N)
    {
      Node min = *orderSet.begin();
      Node max = *orderSet.rbegin();
      if (!set || (max.value - min.value < b - a))  // 统计最小值
      {
        set = true;
        a = min.value;
        b = max.value;
      }
      // 把set的最小值弹出，同时把这个最小值归属的行数组的下一个元素加入set
      // 比如第0行的1出序列，就把5加入set
      min = *orderSet.begin();
      orderSet.erase(orderSet.begin());
      int arrid = min.arrid;
      int index = min.index + 1;
      if (index != nums[arrid].size())
      {
        orderSet.emplace(Node(nums[arrid][index], arrid, index));
      }
    }
    return vector<int>{a, b};
  }
};
