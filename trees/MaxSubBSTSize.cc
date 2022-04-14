#include <cstdint>
#include <iostream>
#include <vector>
#include <queue>

// 找到子树中节点最多的二叉平衡树

using namespace std;

typedef struct Node
{
  int value;
  Node* left;
  Node* right;
}Node;

void in(Node* head, vector<Node*>& arr)
{
  if (head == NULL)
  {
    return;
  }
  in(head->left, arr);
  arr.push_back(head);
  in(head->right, arr);
}

int getBSTSize(Node* head)
{
  if (head == NULL)
  {
    return 0;
  }
  vector<Node*> arr;
  in(head, arr);
  for (int i = 1; i < arr.size(); i++)
  {
    if (arr.at(i)->value <= arr.at(i - 1)->value)
    {
      return 0;
    }
  }
  return arr.size();
}

int maxSubBSTSize1(Node* head)
{
  if (head == NULL)
  {
    return 0;
  }
  int h = getBSTSize(head); // 以当前节点为根的树符合二叉查找树
  if (h != 0)
  {
    return h;
  }
  return std::max(maxSubBSTSize1(head->left), maxSubBSTSize1(head->right));
}

typedef struct Info
{
  int maxBSTSubtreeSize; // 符合二叉搜索树的最大子树的节点个数
  int allSize; // 树中元素的个数
  int max; // 树中元素最大值
  int min; // 树中元素最小值

  Info(int m, int a, int ma, int mi)
  {
    maxBSTSubtreeSize = m;
    allSize = a;
    max = ma;
    min = mi;
  }
}Info;

Info process(Node* x)
{
  if (x == NULL)
  {
    return Info(0, 0, INT32_MIN, INT32_MAX);
  }
  Info leftInfo = process(x->left);
  Info rightInfo = process(x->right);
  int max = x->value;
  int min = x->value;
  int allSize = 1;

  // 统计左子树
  max = std::max(leftInfo.max, max);
  min = std::min(leftInfo.min, min);
  allSize += leftInfo.allSize;
  // 统计右子树
  max = std::max(rightInfo.max, max);
  min = std::min(rightInfo.min, min);
  allSize += rightInfo.allSize;

  int p1 = leftInfo.maxBSTSubtreeSize;
  int p2 = rightInfo.maxBSTSubtreeSize;
  int p3 = 0;
  bool leftBST = leftInfo.maxBSTSubtreeSize == leftInfo.allSize;
  bool rightBST = rightInfo.maxBSTSubtreeSize == rightInfo.allSize;
  if (leftBST && rightBST) // 左、右子树都是二叉搜索树
  {
    bool leftMaxLessX = leftInfo.max < x->value;
    bool rightMinMoreX = x->value < rightInfo.min;
    if (leftMaxLessX && rightMinMoreX) // 当前节点加入也符合
    {
      p3 = leftInfo.allSize + rightInfo.allSize + 1;
    }
  }
  return Info(std::max(p1, std::max(p2, p3)), allSize, max, min);
}

int maxSubBSTSize2(Node* head)
{
  if(head == NULL)
  {
    return 0;
  }
  return process(head).maxBSTSubtreeSize;
}
