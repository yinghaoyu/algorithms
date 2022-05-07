#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <stdbool.h>

// 验证树是不是满二叉树

#define MAX_SEED 1024
#define HALF_SEED (MAX_SEED >> 1)

using namespace std;

class IsFull
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

    static int h(Node* head)
    {
      if (head == nullptr)
      {
        return 0;
      }
      return std::max(h(head->left), h(head->right)) + 1;
    }

    static int n(Node* head)
    {
      if (head == nullptr)
      {
        return 0;
      }
      return n(head->left) + n(head->right) + 1;
    }

    static bool isFull1(Node* head)
    {
      if (head == nullptr)
      {
        return true;
      }
      int height = h(head);
      int nodes = n(head);
      return (1 << height) - 1 == nodes; // 满二叉树的条件是2^h - 1 = n
    }

    class Info
    {
      public:
        int height;
        int nodes;

        Info(int h, int n)
        {
          height = h;
          nodes = n;
        }
    };

    static Info process(Node* head)
    {
      if (head == nullptr)
      {
        return Info(0, 0);
      }
      Info leftInfo = process(head->left);
      Info rightInfo = process(head->right);
      int height = std::max(leftInfo.height, rightInfo.height) + 1;
      int nodes = leftInfo.nodes + rightInfo.nodes + 1;
      return Info(height, nodes);
    }

    static bool isFull2(Node* head)
    {
      if (head == nullptr)
      {
        return true;
      }
      Info all = process(head);
      return (1 << all.height) - 1 == all.nodes;
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
       if (isFull1(head) != isFull2(head))
       {
         cout << "Oops!" << endl;
       }
     }
     cout << "finish!" << endl;
    }
};

int main()
{
  IsFull::test();
  return 0;
}
