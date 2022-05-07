#include <iostream>
#include <random>
#include <stdbool.h>
#include <vector>
#include <queue>

// 验证树是不是完全二叉树

#define MAX_SEED 1024
#define HALF_SEED (MAX_SEED >> 1)

using namespace std;

class IsCBT
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

    // UnRecursive验证二叉树是不是完全二叉树
    static bool isCBT1(Node* head)
    {
      if (head == nullptr)
      {
        return true;
      }
      queue<Node*> queue;
      // 是否遇到过左右两个孩子不双全的节点
      bool leaf = false;
      Node* l = nullptr;
      Node* r = nullptr;
      queue.push(head);
      while (!queue.empty())
      {
        head = queue.front();
        queue.pop();
        l = head->left;
        r = head->right;
        if (
            // 如果遇到了不双全的节点之后，又发现当前节点不是叶节点
            (leaf && (l != nullptr || r != nullptr)) // 从遇到的第二个叶子节点开始，就必须左右都没有节点
            ||
            (l == nullptr && r != nullptr)  // 不双全的必须先要有左节点，再有右节点
           )
        {
          return false;
        }
        if (l != nullptr)
        {
          queue.push(l);
        }
        if (r != nullptr)
        {
          queue.push(r);
        }
        if (l == nullptr || r == nullptr)  // 第一次遇到叶子节点开始，leaf就变成了true
        {
          leaf = true;
        }
      }
      return true;
    }

    // 对每一棵子树，是否是满二叉树、是否是完全二叉树、高度
    class Info
    {
      public:
        bool isFull;
        bool isCBT;
        int height;

        Info(bool full, bool cbt, int h)
        {
          isFull = full;
          isCBT = cbt;
          height = h;
        }
    };

    static Info process(Node* X)
    {
      if (X == nullptr)
      {
        return Info(true, true, 0);
      }
      Info leftInfo = process(X->left);
      Info rightInfo = process(X->right);

      int height = std::max(leftInfo.height, rightInfo.height) + 1;

      bool isFull = leftInfo.isFull
        &&
        rightInfo.isFull
        && leftInfo.height == rightInfo.height;

      bool isCBT = false;
      if (isFull)  // 如果是满二叉树，一定是完全二叉树
      {
        isCBT = true;
      }
      else
      {
        // 以x为头整棵树，不满
        if (leftInfo.isCBT && rightInfo.isCBT)
        {
          if (leftInfo.isCBT
              && rightInfo.isFull
              && leftInfo.height == rightInfo.height + 1)
          {
            // 左子树是完全、右子树是满，且左子树高度比右子树高1
            isCBT = true;
          }
          if (leftInfo.isFull
              &&
              rightInfo.isFull
              && leftInfo.height == rightInfo.height + 1)
          {
            // 左子树是满，右子树是满，且左子树高度比右子树高1
            isCBT = true;
          }
          if (leftInfo.isFull
              && rightInfo.isCBT && leftInfo.height == rightInfo.height)
          {
            // 左子树是满，右子树是完全，且左右子树高度相等
            isCBT = true;
          }
        }
      }
      return Info(isFull, isCBT, height);
    }

    // Recursive验证树是否是满二叉树或者完全二叉树
    static bool isCBT2(Node* head)
    {
      if (head == nullptr)
      {
        return true;
      }
      return process(head).isCBT;
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
      int maxLevel = 5;
      int maxValue = 100;
      int testTimes = 10000;
      for (int i = 0; i < testTimes; i++)
      {
        Node* head = generateRandomBST(maxLevel, maxValue);
        if (isCBT1(head) != isCBT2(head))
        {
          cout << "Oops!" << endl;
        }
      }
      cout << "finish!" << endl;
    }
};

int main()
{
  IsCBT::test();
  return 0;
}
