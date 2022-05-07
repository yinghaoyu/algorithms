#include <random>
#include <iostream>
#include <stdbool.h>
#include <vector>
#include <queue>

// 验证是不是平衡二叉树

#define MAX_SEED 1024
#define HALF_SEED (MAX_SEED >> 1)

using namespace std;

class IsBalanced
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

    static int process1(Node* head, bool& ans)
    {
      if (!ans || head == nullptr)
      {
        return -1;
      }
      int leftHeight = process1(head->left, ans);
      int rightHeight = process1(head->right, ans);
      if (std::abs(leftHeight - rightHeight) > 1)  // 只要高度差大于1就是非平衡二叉树
      {
        ans = false;
      }
      return std::max(leftHeight, rightHeight) + 1;
    }

    // Recursive验证是否平衡
    static bool isBalanced1(Node* head)
    {
      bool ans = true;
      process1(head, ans);
      return ans;
    }

    class Info
    {
      public:
        bool isBalanced;
        int height;

        Info(bool i, int h)
        {
          isBalanced = i;
          height = h;
        }
    };

    static Info process(Node* x)
    {
      if(x == nullptr)
      {
        return Info(true, 0);
      }
      Info leftInfo = process(x->left);
      Info rightInfo = process(x->right);
      int height = std::max(leftInfo.height, rightInfo.height)  + 1;
      bool isBalanced = true;
      if(!leftInfo.isBalanced)
      {
        isBalanced = false;
      }
      if(!rightInfo.isBalanced)
      {
        isBalanced = false;
      }
      if(std::abs(leftInfo.height - rightInfo.height) > 1)
      {
        isBalanced = false;
      }
      return Info(isBalanced, height);
    }

    // Recursive验证是否平衡
    static bool isBalanced2(Node* head)
    {
      return process(head).isBalanced;
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
       if (isBalanced1(head) != isBalanced2(head))
       {
         cout << "Oops!" <<endl;
       }
     }
     cout << "finish!" <<endl;
    }
};

int main()
{
  IsBalanced::test();
  return 0;
}
