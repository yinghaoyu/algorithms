#include <iostream>
#include <random>
#include <string>

using namespace std;

#define MAX_SEED 1024
#define MIN_SEED 16

// 如果一个节点X，它左树结构和右树结构完全一样，那么我们说以X为头的子树是相等子树
// 给定一棵二叉树的头节点head，返回head整棵树上有多少棵相等子树
// 例如下面这棵树：
//        6             6符合，左右子树完全一样
//      ↙   ↘
//     1     1         1都不符合，因为左右子树不相等
//    ↙ ↘   ↙ ↘
//   2   3  2  3       2和3都符合，因为左右子树都为空，也算相等
// 综上，总共有5个节点符合条件，因此返回5
class LeftRightSameTreeNumber
{
 public:
  class Node
  {
   public:
    int value;

    Node *left;

    Node *right;

    Node(int v) { value = v; }
  };

  // 时间复杂度O(N * logN)
  static int sameNumber1(Node *head)
  {
    if (head == nullptr)
    {
      return 0;
    }
    return sameNumber1(head->left) + sameNumber1(head->right) + (same(head->left, head->right) ? 1 : 0);
  }

  static bool same(Node *h1, Node *h2)
  {
    if (h1 == nullptr ^ h2 == nullptr)
    {
      return false;
    }
    if (h1 == nullptr && h2 == nullptr)
    {
      return true;
    }
    // 两个都不为空
    return h1->value == h2->value && same(h1->left, h2->left) && same(h1->right, h2->right);
  }

  // 利用hash，将判断树的结构降为两个定长的hash code比较，冲突概率极小
  // 时间复杂度降为O(N)
  static int sameNumber2(Node *head)
  {
    hash<string> hash;
    return process(head, hash).ans;
  }

  class Info
  {
   public:
    int ans;

    int hash;

    Info(int a, int h)
    {
      ans = a;
      hash = h;
    }
  };

  static Info process(Node *head, hash<string> &hash)
  {
    if (head == nullptr)
    {
      return Info(0, hash("#,"));
    }
    Info l = process(head->left, hash);
    Info r = process(head->right, hash);
    int ans = (l.hash == r.hash ? 1 : 0) + l.ans + r.ans;
    int h = hash(std::to_string(head->value) + "," + std::to_string(l.hash) + std::to_string(r.hash));
    return Info(ans, h);
  }

  static Node *randomBinaryTree(int restLevel, int maxValue)
  {
    if (restLevel == 0)
    {
      return nullptr;
    }
    Node *head = getRandom(0, MAX_SEED) < MIN_SEED ? nullptr : new Node((int) (getRandom(0, maxValue)));
    if (head != nullptr)
    {
      head->left = randomBinaryTree(restLevel - 1, maxValue);
      head->right = randomBinaryTree(restLevel - 1, maxValue);
    }
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
    int maxLevel = 8;
    int maxValue = 4;
    int testTime = 100000;
    cout << "测试开始" << endl;
    for (int i = 0; i < testTime; i++)
    {
      Node *head = randomBinaryTree(maxLevel, maxValue);
      int ans1 = sameNumber1(head);
      int ans2 = sameNumber2(head);
      if (ans1 != ans2)
      {
        cout << "出错了！" << endl;
        cout << ans1 << endl;
        cout << ans2 << endl;
      }
    }
    cout << "测试结束" << endl;
  }
};

int main()
{
  LeftRightSameTreeNumber::test();
  return 0;
}
