#include <iostream>
#include <vector>

using namespace std;

// 本题测试链接 : https://leetcode.com/problems/construct-binary-search-tree-from-preorder-traversal/

class ConstructBinarySearchTreeFromPreorderTraversal
{
  // 不用提交这个类
 public:
  class TreeNode
  {
   public:
    int val;

    TreeNode *left;

    TreeNode *right;

    TreeNode() {}

    TreeNode(int val) { this->val = val; }

    TreeNode(int val, TreeNode *left, TreeNode *right)
    {
      this->val = val;
      this->left = left;
      this->right = right;
    }
  };

  // 提交下面的方法
  static TreeNode *bstFromPreorder1(vector<int> &pre)
  {
    if (pre.size() == 0)
    {
      return nullptr;
    }
    return process1(pre, 0, pre.size() - 1);
  }

  static TreeNode *process1(vector<int> *pre, int L, int R)
  {
    if (L > R)
    {
      return nullptr;
    }
    int firstBig = L + 1;
    for (; firstBig <= R; firstBig++)
    {
      if (pre[firstBig] > pre[L])
      {
        break;
      }
    }
    TreeNode *head = new TreeNode(pre[L]);
    head->left = process1(pre, L + 1, firstBig - 1);
    head->right = process1(pre, firstBig, R);
    return head;
  }

  // 已经是时间复杂度最优的方法了，但是常数项还能优化
  static TreeNode *bstFromPreorder2(vector<int> &pre)
  {
    if (pre.size() == 0)
    {
      return nullptr;
    }
    int N = pre.size();
    vector<int> nearBig(N);
    for (int i = 0; i < N; i++)
    {
      nearBig[i] = -1;
    }
    vector<int> stack;
    for (int i = 0; i < N; i++)
    {
      while (!stack.empty() && pre[stack.back()] < pre[i])
      {
        int v = stack.back();
        stack.pop_back();
        nearBig[v] = i;
      }
      stack.push_back(i);
    }
    return process2(pre, 0, N - 1, nearBig);
  }

  static TreeNode *process2(vector<int> &pre, int L, int R, vector<int> &nearBig)
  {
    if (L > R)
    {
      return nullptr;
    }
    int firstBig = (nearBig[L] == -1 || nearBig[L] > R) ? R + 1 : nearBig[L];
    TreeNode *head = new TreeNode(pre[L]);
    head->left = process2(pre, L + 1, firstBig - 1, nearBig);
    head->right = process2(pre, firstBig, R, nearBig);
    return head;
  }

  // 最优解
  static TreeNode *bstFromPreorder3(vector<int> &pre)
  {
    if (pre.size() == 0)
    {
      return nullptr;
    }
    int N = pre.size();
    vector<int> nearBig(N);
    for (int i = 0; i < N; i++)
    {
      nearBig[i] = -1;
    }
    vector<int> stack(N);
    int size = 0;
    for (int i = 0; i < N; i++)
    {
      while (size != 0 && pre[stack[size - 1]] < pre[i])
      {
        nearBig[stack[--size]] = i;
      }
      stack[size++] = i;
    }
    return process3(pre, 0, N - 1, nearBig);
  }

  static TreeNode *process3(vector<int> &pre, int L, int R, vector<int> &nearBig)
  {
    if (L > R)
    {
      return nullptr;
    }
    int firstBig = (nearBig[L] == -1 || nearBig[L] > R) ? R + 1 : nearBig[L];
    TreeNode *head = new TreeNode(pre[L]);
    head->left = process3(pre, L + 1, firstBig - 1, nearBig);
    head->right = process3(pre, firstBig, R, nearBig);
    return head;
  }
};
