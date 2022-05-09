#include <iostream>
#include <stdbool.h>
#include <random>
#include <vector>

#define MAX_SEED 1024
#define HALF_SEED (MAX_SEED >> 1)

using namespace std;

// 获取子树中最大的二叉搜索树，返回头结点

class MaxSubBSTHead
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

    static Node* maxSubBSTHead1(Node* head)
    {
      if (head == nullptr)
      {
        return nullptr;
      }
      if (getBSTSize(head) != 0)
      {
        return head;
      }
      Node* leftAns = maxSubBSTHead1(head->left);
      Node* rightAns = maxSubBSTHead1(head->right);
      return getBSTSize(leftAns) >= getBSTSize(rightAns) ? leftAns : rightAns;
    }

    static Node* maxSubBSTHead2(Node* head)
    {
      if (head == nullptr)
      {
        return nullptr;
      }
      return process(head).maxSubBSTHead;
    }

    // 每一棵子树
    class Info
    {
      public:
        Node* maxSubBSTHead;  // 二叉搜索树的根节点
        int maxSubBSTSize;  // 二叉搜索树子树的size
        int min;  // 最大值
        int max;  // 最小值

        Info(Node* h, int size, int mi, int ma)
        {
          maxSubBSTHead = h;
          maxSubBSTSize = size;
          min = mi;
          max = ma;
        }
    };

    static Info process(Node* X)
    {
      if (X == nullptr)
      {
        return Info(nullptr, 0, INT32_MAX, INT32_MIN);
      }
      Info leftInfo = process(X->left);
      Info rightInfo = process(X->right);
      int min = X->value;
      int max = X->value;
      Node* maxSubBSTHead = nullptr;
      int maxSubBSTSize = 0;

      // 计算左子树的结果
      min = std::min(min, leftInfo.min);
      max = std::max(max, leftInfo.max);
      maxSubBSTHead = leftInfo.maxSubBSTHead;
      maxSubBSTSize = leftInfo.maxSubBSTSize;

      // 计算右子树的结果
      min = std::min(min, rightInfo.min);
      max = std::max(max, rightInfo.max);
      if (rightInfo.maxSubBSTSize > maxSubBSTSize)
      {
        maxSubBSTHead = rightInfo.maxSubBSTHead;
        maxSubBSTSize = rightInfo.maxSubBSTSize;
      }

      if ((leftInfo.maxSubBSTHead == X->left && leftInfo.max < X->value)
          && (rightInfo.maxSubBSTHead == X->right && rightInfo.min > X->value))
      {
        maxSubBSTHead = X;
        maxSubBSTSize = leftInfo.maxSubBSTSize + rightInfo.maxSubBSTSize + 1;
      }
      return Info(maxSubBSTHead, maxSubBSTSize, min, max);
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
        if (maxSubBSTHead1(head) != maxSubBSTHead2(head))
        {
          cout << "Oops!" << endl;
        }
      }
      cout << "finish!" << endl;
    }
};

int main()
{
  MaxSubBSTHead::test();
  return 0;
}
