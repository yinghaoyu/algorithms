#include <iostream>
#include <vector>
using namespace std;

// 背景：为什么引入树状数组？
// 普通数组求区间和，修改单个元素时间复杂度O(1)，求区间和时间复杂度O(n)
// 前缀和数组求区间和，修改单个元素时间复杂度O(n)，求区间和时间复杂度O(1)
// 树状数组就是对两者的折中，思想是把区间管理分成 m 个子区间(区间存在重叠)，
// 然后在子区间维护信息，使得修改单个元素和区间查询都为 O(logN)
// 原数组 A 对应整个区间，树状数组 C 对应子区间
// 可以这么理解，树状数组的核心关键是二进制的重叠子结构
// C[i] 管理的区间是 (i - lowBit(i), i]，直到 i - lowBit(i) 为 0 停止
// 例如 i = 8(1000)，它管理的区间是(0, 1000]
//      i = 7(0111)，它管理的区间是(0110, 0111], (0100, 0110], (0, 0100] 
// 
//                                 → → → → → → → → → → (0,1000]
//                               ↗               ↗  ↑      ↑
//                    → → → → (0,100]          ↗    ↑      ↑
//                  ↗         ↗ ↑            ↗      ↑      ↑
//             →  (0,10)    ↗   ↑       (100, 110]  ↑      ↑
//           ↗      ↑     ↗     ↑     ↗      ↑      ↑      ↑
// C数组   (0,1]    ↑  (10,11]  ↑ (100,101]  ↑  (110,111]  ↑
//          ↑       ↑     ↑     ↑     ↑      ↑      ↑      ↑
// A数组   1       2      3     4     5      6      7      8

// 注意 A 数组下标从 0 开始， C 数组下标从 1 开始
// 当要查询原组区间和A[0...i]时(C对应下标1...i+1)，只需要把管理的区间求和 ∑(i>0) C[i - lowBit(i)]
// 当要更新原数组A[i]时(C对应下标i+1)，只需要把关联区间都更新，C[i + lowBit(i)] = diff (i+lowBit(i) < C.size())
// leetcode第307题
class NumArray
{
 public:
  vector<int> A;  // 原数组
  vector<int> C;  // 树状数组

  int lowBit(int x) { return x & (-x); }

  void add(int index, int u)
  {
    for (int i = index; i < C.size(); i += lowBit(i))
    {
      C[i] += u;
    }
  }

  int query(int index)
  {
    int sum = 0;
    for (int i = index; i > 0; i -= lowBit(i))
    {
      sum += C[i];
    }
    return sum;
  }

  NumArray(vector<int> &nums) : A(nums)
  {
    // 树状数组下标从1开始，防止lowBit结果为0，导致不能退出for循环
    C = vector<int>(A.size() + 1, 0);
    for (int i = 0; i < A.size(); i++)
    {
      add(i + 1, A[i]);
    }
  }

  void update(int index, int val)
  {
    int diff = val - A[index];
    add(index + 1, diff);
    A[index] = val;
  }

  // 这里的区间为[left, right]，包含了A[left]的值
  int sumRange(int left, int right)
  {
    // 等价于 query(right+1) - query(left+1) + A[left + 1];
    return query(right + 1) - query(left);
  }
};

/**
 * Your NumArray object will be instantiated and called as such:
 * NumArray* obj = new NumArray(nums);
 * obj->update(index,val);
 * int param_2 = obj->sumRange(left,right);
 */

/*************** Way 2 *******************/
// 下标从1开始！
class IndexTree
{
 private:
  vector<int> tree;
  int N;

 public:
  // 0位置弃而不用！
  IndexTree(int size)
  {
    N = size;
    tree = vector<int>(N + 1);
  }

  // 1~index 累加和是多少？
  int sum(int index)
  {
    int ret = 0;
    while (index > 0)
    {
      ret += tree[index];
      index -= index & -index;
    }
    return ret;
  }

  // index & -index : 提取出index最右侧的1出来
  // index :           0011001000
  // index & -index :  0000001000
  void add(int index, int d)
  {
    while (index <= N)
    {
      tree[index] += d;
      index += index & -index;
    }
  }
};
