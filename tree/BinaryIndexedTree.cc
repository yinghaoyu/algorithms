#include <iostream>
#include <vector>
using namespace std;

// leetcode第307题
class NumArray {
public:
    vector<int> A;  // 原数组
    vector<int> C;  // 树状数组
    
    int lowBit(int x)
    {
      return x & (-x);
    }

    void add(int index, int u)
    {
      for(int i = index; i < C.size(); i += lowBit(i))
      {
        C[i] += u;
      }
    }

    int query(int index)
    {
      int sum = 0;
      for(int i = index; i > 0; i -= lowBit(i))
      {
        sum += C[i];
      }
      return sum;
    }

    NumArray(vector<int>& nums):A(nums) {
      // 树状数组下标从1开始，防止lowBit结果为0，导致不能退出for循环
      C = vector<int>(A.size() + 1 , 0);
      for(int i = 0; i < A.size(); i++)
      {
        add(i+1, A[i]);
      }
    }
    
    void update(int index, int val) {
      int diff = val - A[index];
      add(index+1, diff);
      A[index] = val;
    }
    
    // 这里的区间为[left, right]，包含了A[left]的值
    int sumRange(int left, int right) {
      // 等价于 query(right+1) - query(left+1) + A[left];
      return query(right+1) - query(left);
    }
};

/**
 * Your NumArray object will be instantiated and called as such:
 * NumArray* obj = new NumArray(nums);
 * obj->update(index,val);
 * int param_2 = obj->sumRange(left,right);
 */
