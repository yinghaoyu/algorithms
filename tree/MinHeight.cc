#include <cstdint>
#include <iostream>
#include <random>

using namespace std;

#define MAX_SEED 1024
#define HALF_SEED (MAX_SEED >> 1)

class MinHeight
{
 public:
  class Node
  {
   public:
    int val;
    Node *left;
    Node *right;

    Node(int x) { val = x; }
  };

  static int minHeight1(Node *head)
  {
    if (head == nullptr)
    {
      return 0;
    }
    return p(head);
  }

  // 返回x为头的树，最小深度是多少
  static int p(Node *x)
  {
    if (x->left == nullptr && x->right == nullptr)
    {
      // 当前只有x节点
      return 1;
    }
    // 左右子树起码有一个不为空
    int leftH = INT32_MAX;
    if (x->left != nullptr)
    {
      leftH = p(x->left);
    }
    int rightH = INT32_MAX;
    if (x->right != nullptr)
    {
      rightH = p(x->right);
    }
    return 1 + std::min(leftH, rightH);
  }

  // 根据morris遍历改写
  static int minHeight2(Node *head)
  {
    if (head == nullptr)
    {
      return 0;
    }
    Node *cur = head;
    Node *mostRight = nullptr;
    int curLevel = 0;
    int minHeight = INT32_MAX;
    while (cur != nullptr)
    {
      mostRight = cur->left;
      if (mostRight != nullptr)
      {
        int rightBoardSize = 1;
        while (mostRight->right != nullptr && mostRight->right != cur)
        {
          rightBoardSize++;
          mostRight = mostRight->right;
        }
        if (mostRight->right == nullptr)
        {
          // 第一次到达
          curLevel++;
          mostRight->right = cur;
          cur = cur->left;
          continue;
        }
        else
        {
          // 第二次到达
          if (mostRight->left == nullptr)
          {
            minHeight = std::min(minHeight, curLevel);
          }
          curLevel -= rightBoardSize;
          mostRight->right = nullptr;
        }
      }
      else
      {  // 只有一次到达
        curLevel++;
      }
      cur = cur->right;
    }
    int finalRight = 1;
    cur = head;
    while (cur->right != nullptr)
    {
      finalRight++;
      cur = cur->right;
    }
    if (cur->left == nullptr && cur->right == nullptr)
    {
      minHeight = std::min(minHeight, finalRight);
    }
    return minHeight;
  }

  // for test
  static Node *generateRandomBST(int maxLevel, int maxValue) { return generate(1, maxLevel, maxValue); }

  // for test
  static Node *generate(int level, int maxLevel, int maxValue)
  {
    if (level > maxLevel || getRandom(0, MAX_SEED) < HALF_SEED)
    {
      return nullptr;
    }
    Node *head = new Node(getRandom(0, maxValue));
    head->left = generate(level + 1, maxLevel, maxValue);
    head->right = generate(level + 1, maxLevel, maxValue);
    return head;
  }

  static int getRandom(int min, int max)
  {
    random_device seed;                            // 硬件生成随机数种子
    ranlux48 engine(seed());                       // 利用种子生成随机数引
    uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
    int res = distrib(engine);                     // 随机数
    return res;
  }

  static void test()
  {
    int treeLevel = 7;
    int nodeMaxValue = 5;
    int testTimes = 100000;
    cout << "test begin" << endl;
    for (int i = 0; i < testTimes; i++)
    {
      Node *head = generateRandomBST(treeLevel, nodeMaxValue);
      int ans1 = minHeight1(head);
      int ans2 = minHeight2(head);
      if (ans1 != ans2)
      {
        cout << "Oops!" << endl;
      }
    }
    cout << "test finish!" << endl;
  }
};

int main()
{
  MinHeight::test();
  return 0;
}
