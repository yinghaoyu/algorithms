#include <iostream>
#include <vector>
#include <queue>
#include <stdbool.h>
#include <random>

// 验证树是不是二叉搜索树

#define MAX_SEED 1024
#define HALF_SEED (MAX_SEED >> 1)

using namespace std;

class IsBST
{
  public:
    class Node
    {
      public:
        int value;
        Node* left;
        Node* right;

        Node (int data)
        {
          this->value = data;
        }
    };

    static void in(Node* head, vector<Node*>& arr)
    {
      if (head == NULL)
      {
        return;
      }
      in(head->left, arr);
      arr.push_back(head);
      in(head->right, arr);
    }

    //Recursive验证是否是二叉查找树
    static bool isBST1(Node* head)
    {
      if (head == NULL)
      {
        return true;
      }
      vector<Node*> arr;
      in(head, arr);
      for (int i = 1; i < arr.size(); i++)
      {
        // 中序遍历一定是有序的
        if (arr.at(i)->value <= arr.at(i - 1)->value)
        {
          return false;
        }
      }
      return true;
    }

    class Info
    {
      public:
        bool isBST;
        int max;
        int min;

        Info(bool i, int ma, int mi)
        {
          isBST = i;
          max = ma;
          min = mi;
        }
    };

    static Info process(Node* x)
    {
      if (x == NULL)
      {
        return Info(true, INT32_MIN, INT32_MAX);
      }
      Info leftInfo = process(x->left);
      Info rightInfo = process(x->right);

      int max = x->value;
      max = std::max(max, leftInfo.max);
      max = std::max(max, rightInfo.max);

      int min = x->value;
      min = std::min(min, leftInfo.min);
      min = std::min(min, rightInfo.min);

      bool isBST = true;
      if (!leftInfo.isBST)  // 左子树必须有序
      {
        isBST = false;
      }
      if (!rightInfo.isBST)  // 右子树必须有序
      {
        isBST = false;
      }
      if (leftInfo.max >= x->value) // 左子树的最大值必须小于当前节点的值
      {
        isBST = false;
      }
      if (rightInfo.min <= x->value) // 右子树的最小值必须大于当前节点的值
      {
        isBST = false;
      }
      return Info(isBST, max, min);
    }

    static bool isBST2(Node* head)
    {
      if (head == NULL)
      {
        return true;
      }
      return process(head).isBST;
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
        return NULL;
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
        if (isBST1(head) != isBST2(head))
        {
          cout << "Oops!" << endl;
        }
      }
      cout << "finish!" << endl;
    }
};

int main()
{
  IsBST::test();
  return 0;
}
