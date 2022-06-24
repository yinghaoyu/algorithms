#include <assert.h>
#include <stdbool.h>
#include <deque>
#include <iostream>
#include <vector>

using namespace std;

// 测试链接：https://leetcode.com/problems/gas-station

// 问题描述：
// 在一条环路上有 n 个加油站，其中第 i 个加油站有汽油 gas[i] 升。
// 你有一辆油箱容量无限的的汽车，从第 i 个加油站开往第 i+1 个加油站需要消耗汽油 cost[i] 升。
// 你从其中的一个加油站出发，开始时油箱为空。
// 给定两个整数数组 gas 和 cost ，如果你可以绕环路行驶一周，则返回出发时加油站的编号，否则返回 -1 。
// 如果存在解，则 保证 它是 唯一 的。

// 输入: gas = [1,2,3,4,5], cost = [3,4,5,1,2]
// 输出: 3
// 解释:
// 从 3 号加油站(索引为 3 处)出发，可获得 4 升汽油。此时油箱有 = 0 + 4 = 4 升汽油
// 开往 4 号加油站，此时油箱有 4 - 1 + 5 = 8 升汽油
// 开往 0 号加油站，此时油箱有 8 - 2 + 1 = 7 升汽油
// 开往 1 号加油站，此时油箱有 7 - 3 + 2 = 6 升汽油
// 开往 2 号加油站，此时油箱有 6 - 4 + 3 = 5 升汽油
// 开往 3 号加油站，你需要消耗 5 升汽油，正好足够你返回到 3 号加油站。
// 因此，3 可为起始索引。

// 输入: gas = [2,3,4], cost = [3,4,3]
// 输出: -1
// 解释:
// 你不能从 0 号或 1 号加油站出发，因为没有足够的汽油可以让你行驶到下一个加油站。
// 我们从 2 号加油站出发，可以获得 4 升汽油。 此时油箱有 = 0 + 4 = 4 升汽油
// 开往 0 号加油站，此时油箱有 4 - 3 + 2 = 3 升汽油
// 开往 1 号加油站，此时油箱有 3 - 3 + 3 = 3 升汽油
// 你无法返回 2 号加油站，因为返程需要消耗 4 升汽油，但是你的油箱只有 3 升汽油。
// 因此，无论怎样，你都不可能绕环路行驶一周。

class GasStation
{
 public:
  // 这个方法的时间复杂度O(N)，额外空间复杂度O(N)
  static int canCompleteCircuit(vector<int> &gas, vector<int> &cost)
  {
    assert(gas.size() == cost.size());
    vector<bool> good = goodArray(gas, cost);
    for (int i = 0; i < gas.size(); i++)
    {
      if (good[i])
      {
        return i;
      }
    }
    return -1;
  }

  static vector<bool> goodArray(vector<int> &g, vector<int> &c)
  {
    int N = g.size();
    int M = N << 1;  // 为了方便计算下标
    vector<int> arr(M);
    for (int i = 0; i < N; i++)
    {
      // arr[0...N-1]为纯能值数组
      // 暴力解法：计算arr[i...i+N]的前缀和数组，只要每项前缀和>=0即符合条件
      // 如果用暴力解法时间复杂度log(N^2)
      arr[i] = g[i] - c[i];
      arr[i + N] = g[i] - c[i];
    }
    for (int i = 1; i < M; i++)
    {
      arr[i] += arr[i - 1];  // 前缀和
    }
    deque<int> w;
    for (int i = 0; i < N; i++)
    {
      // 先对前N个元素构建单调递增栈
      while (!w.empty() && arr[w.back()] >= arr[i])
      {
        w.pop_back();
      }
      w.push_back(i);
    }
    vector<bool> ans(N);
    for (int offset = 0, i = 0, j = N; j < M; offset = arr[i++], j++)
    {
      // arr[i...j]之间的最小值
      // 这个区间刚好表示原数组环形一圈的轨迹
      // offset为arr[i-1]
      // 这里判断每一项前缀和是否都>=0，单调栈里如果最小值都满足，那么整个区间都满足了
      // 这里将时间复杂度降到log(N)
      if (arr[w.front()] - offset >= 0)
      {
        ans[i] = true;
      }
      // 删除即将过期的，因为马上要i++了
      if (w.front() == i)
      {
        w.pop_front();
      }
      // 从j = N开始往右扩，保证单调递增
      while (!w.empty() && arr[w.back()] >= arr[j])
      {
        w.pop_back();
      }
      w.push_back(j);
    }
    return ans;
  }
};
