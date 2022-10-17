#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;
// 问题描述：
// 两种颜色的球，蓝色和红色，都按1～n编号，共计2n个
// 为方便放在一个数组中，红球编号取负，篮球不变，并打乱顺序，
// 要求同一种颜色的球按编号升序排列，可以进行如下操作：
// 交换相邻两个球，求最少操作次数。
// [3,-3,1,-4,2,-2,-1,4]
// 最终交换结果为
// [1,2,3,-1,-2,-3,-4,4]
// 最少交换次数为10
// n <= 1000
//
// 问题分析：
// 根据最终结果分析，从右向左，最大的编号一定在最右侧
// 开始的决策：
// 把4移动到最右侧，操作0次 --> [3,-3,1,-4,2,-2,-1,4]
// 或者
// 把-4移动到最右侧，操作4次 --> [3,-3,1,2,-2,-1,4,-4]
// 取两个操作次数的最小值
// 然后把3或者-3移到最右侧（排除上一步的位置，上一步移动到7位置，这步移动到6位置），依次类推
// 为什么用移动？因为移动不会破坏中间元素的有序性，交换次数更少
//
// 为了高效查询每个数移动到最右边需要的次数，使用索引树（单点更新 + 区间查询）
// 初始时idx所有元素值为 1 ，每当移动 value 时，获取 value 的位置
// 移动的次数为(当前value的位置到最右侧之间 1 的总数 - 1)，同时把对应的索引清0
// arr  [3,-3,1,-4,2,-2,-1,4]
// idx  [1, 1,1, 1,1, 1, 1,1]
// 假设移动 -4 到最右边，实际上不用改变arr，而是将idx.at(value) = 0
// arr  [3,-3,1,2,-2,-1,4,-4]（理想情况，实际上arr还是初始状态）
// arr  [3,-3,1,-4,2,-2,-1,4]
// idx  [1, 1,1, 0,1, 1, 1,1]
// 前面移动 -4 ，移动次数为5 - 1 = 4 次，对应idx位置上清0
// 后续移动 3 ，移动次数 7 - 1 = 6次，可以发现没有去计算 -4 位置
// 因为 -4 在前面已经被移到最右侧了，把对应的idx清 0 刚好排除
// 这符合单点更新 + 区间查询，利用索引树时间复杂度可降到log(N)
class TwoTeamsSortedMinSwap
{
  //  public static int zuo(int lastA, int lastB, int[] arr) {
  //    if (lastA == 0 && lastB == 0) {
  //      return 0;
  //    }
  //    // lastA != 0 || lastB != 0
  //    if (lastA == 0) {
  //      // lastB
  //      int curCost = lastB来到此时的终止位置的代价;
  //      int next = zuo(lastA, lastB - 1, arr);
  //      return curCost + next;
  //    }
  //    if(lastB == 0) {
  //      int curCost = lastA来到此时的终止位置的代价;
  //      int next = zuo(lastA - 1, lastB, arr);
  //      return curCost + next;
  //    }
  //    // lastA, lastB
  //    // 让lastA来到最后
  //    int p1 = Integer.MAX_VALUE;
  //    int lastAComeCost = lastA来到此时的终止位置的代价;
  //    int next1 = zuo(lastA - 1, lastB, arr);
  //    p1 = lastAComeCost + next1;
  //    // 让lastB来到最后
  //    int p2 = Integer.MAX_VALUE;
  //    int lastBComeCost = lastB来到此时的终止位置的代价;
  //    int next2 = zuo(lastA, lastB - 1, arr);
  //    p2 = lastBComeCost + next2;
  //    return Math.min(p1, p2);
  //  }

  // [3,-3,1,-4,2,-2,-1,4]
  // -3 -4 -2 -1 -> -1 -2 -3 -4
  // 3 1 2 4 -> 1 2 3 4
  class IndexTree
  {
   private:
    vector<int> tree;
    int N;

   public:
    IndexTree(int size)
    {
      N = size;
      tree = vector<int>(N + 1);
    }

    void add(int i, int v)
    {
      i++;
      while (i <= N)
      {
        tree[i] += v;
        i += i & -i;
      }
    }

    // 这里的区间为[l, r]，包含了tree[l + 1]的值
    // 由于下标从1开始，这里等价于sum(r + 1) - sum(l + 1) + tree[l + 1]
    int sum(int l, int r) { return l == 0 ? sum(r + 1) : (sum(r + 1) - sum(l)); }

   private:
    int sum(int index)
    {
      int ans = 0;
      while (index > 0)
      {
        ans += tree[index];
        index -= index & -index;
      }
      return ans;
    }
  };

 public:
  static void test()
  {
    vector<int> arr = {3, -3, 1, -4, 2, -2, -1, 4};
    cout << minSwaps(arr) << endl;
  }

  static int minSwaps(vector<int> arr)
  {
    int n = arr.size();
    unordered_map<int, int> map;
    int topA = 0;
    int topB = 0;
    for (int i = 0; i < n; i++)
    {
      if (arr[i] > 0)
      {
        topA = std::max(topA, arr[i]);
      }
      else
      {
        topB = std::max(topB, std::abs(arr[i]));
      }
      map.emplace(arr[i], i);
    }
    IndexTree it(n);
    for (int i = 0; i < n; i++)
    {
      it.add(i, 1);
    }
    return f(topA, topB, it, n - 1, map);
  }

  // 可以改二维动态规划！
  // 因为it的状态，只由topA和topB决定
  // 所以it的状态不用作为可变参数！
  static int f(int lastA,
               int lastB,
               IndexTree &it,                // 支持快速的距离计算
               int end,                      // 不变！永远n-1！
               unordered_map<int, int> &map  // 位置表
  )
  {
    if (lastA == 0 && lastB == 0)
    {
      // 说明两种颜色的球都已经排好了
      return 0;
    }
    int p1 = INT32_MAX;
    int p2 = INT32_MAX;
    int index, cost, next;
    if (lastA != 0)
    {
      // 查出原数组中，lastA在哪？
      index = map.at(lastA);
      //
      cost = it.sum(index, end) - 1;
      // 既然搞定了lastA，indexTree
      it.add(index, -1);
      next = f(lastA - 1, lastB, it, end, map);
      it.add(index, 1);
      p1 = cost + next;
    }
    if (lastB != 0)
    {
      index = map.at(-lastB);
      cost = it.sum(index, end) - 1;
      it.add(index, -1);
      next = f(lastA, lastB - 1, it, end, map);
      it.add(index, 1);
      p2 = cost + next;
    }
    return std::min(p1, p2);
  }
};

int main()
{
  TwoTeamsSortedMinSwap::test();
  return 0;
}
