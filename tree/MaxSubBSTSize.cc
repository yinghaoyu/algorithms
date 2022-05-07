#include <cstdint>
#include <iostream>
#include <vector>
#include <queue>
#include <random>

// 找到子树中节点最多的二叉平衡树

#define MAX_SEED 1024
#define HALF_SEED (MAX_SEED >> 1)

using namespace std;


class MaxSubBSTSize
{
  public:
    class Node
    {
      public:
        int value;
        Node* left;
        Node* right;

        Node(int data)
        {
          this->value = data;
        }
    };

    static void in(Node* head, vector<Node*>& arr)
    {
      if (head == nullptr)
      {
        return;
      }
      in(head->left, arr);
      arr.push_back(head);
      in(head->right, arr);
    }

    static int getBSTSize(Node* head)
    {
      if (head == nullptr)
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

    static int maxSubBSTSize1(Node* head)
    {
      if (head == nullptr)
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

    class Info
    {
      public:
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
    };

    static Info process(Node* x)
    {
      if (x == nullptr)
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

    static int maxSubBSTSize2(Node* head)
    {
      if(head == nullptr)
      {
        return 0;
      }
      return process(head).maxBSTSubtreeSize;
    }

    static int getRandom(int min, int max)
    {
      random_device seed;  // 硬件生成随机数种子
      ranlux48 engine(seed());  // 利用种子生成随机数引
      uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
      int res = distrib(engine);  // 随机数
      return res;
    }

    // for test
    static Node* generateRandomBST(int maxLevel, int maxValue)
    {
      return generate(1, maxLevel, maxValue);
    }

    // for test
    static Node* generate(int level, int maxLevel, int maxValue)
    {
      if (level > maxLevel || getRandom(0, MAX_SEED) < HALF_SEED)
      {
        return nullptr;
      }
      Node* head = new Node(getRandom(0, maxValue));
      head->left = generate(level + 1, maxLevel, maxValue);
      head->right = generate(level + 1, maxLevel, maxValue);
      return head;
    }

    static void test()
    {
      int maxLevel = 4;
      int maxValue = 100;
      int testTimes = 10000;
      for (int i = 0; i < testTimes; i++)
      {
        Node* head = generateRandomBST(maxLevel, maxValue);
        if (maxSubBSTSize1(head) != maxSubBSTSize2(head))
        {
          cout << "Oops!" <<endl;
        }
      }
      cout << "finish!" <<endl;
    }
};

int main()
{
  MaxSubBSTSize::test();
  return 0;
}
