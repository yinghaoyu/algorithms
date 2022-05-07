#include <iostream>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stdbool.h>

#define MAX_SEED 1024
#define HALF_SEED (MAX_SEED >> 1)

using namespace std;

// 二叉树的最大距离

// 给定一个二叉树，任何两个节点之间都存在距离，返回整个二叉树的最大距离。两个节点的距离表示从一个节点到另一个节点最短路径上的节点数（包含两个节点自己）

// 分析：以 X 为头的二叉树，它的最大距离可能有两类，不经过 X ，经过X。
//      情况一       |       情况二
//          X        |          X
//         ↙ ↘       |         ↙ ↘
//        *   *      |        *   *
//       ↙ ↘         |       ↙     ↘
//      *   *        |      *       *
//     ↙     ↘       |     ↙         ↘
//    *       *      |    *           *
//   ↙         ↘     |   ↙             ↘
//  *           *    |  *               *

class MaxDistance
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

    static int maxDistance1(Node* head)
    {
      if (head == nullptr)
      {
        return 0;
      }
      vector<Node*> arr = getPrelist(head); // 前序遍历
      // 节点指针 key 节点的父指针 value
      unordered_map<Node*, Node*> parentMap = getParentMap(head);
      int max = 0;
      for (int i = 0; i < arr.size(); i++)
      {
        for (int j = i; j < arr.size(); j++)
        {
          max = std::max(max, distance(parentMap, arr.at(i), arr.at(j)));
        }
      }
      return max;
    }

    static vector<Node*> getPrelist(Node* head)
    {
      vector<Node*> arr;
      fillPrelist(head, arr);
      return arr;
    }

    static void fillPrelist(Node* head, vector<Node*>& arr)
    {
      if (head == nullptr)
      {
        return;
      }
      arr.push_back(head);
      fillPrelist(head->left, arr);
      fillPrelist(head->right, arr);
    }

    static unordered_map<Node*, Node*> getParentMap(Node* head)
    {
      unordered_map<Node*, Node*> map;
      map.emplace(head, nullptr);
      fillParentMap(head, map);
      return map;
    }

    static void fillParentMap(Node* head, unordered_map<Node*, Node*>& parentMap)
    {
      if (head->left != nullptr)
      {
        parentMap.emplace(head->left, head);
        fillParentMap(head->left, parentMap);
      }
      if (head->right != nullptr)
      {
        parentMap.emplace(head->right, head);
        fillParentMap(head->right, parentMap);
      }
    }

    static int distance(unordered_map<Node*, Node*> parentMap, Node* o1, Node* o2)
    {
      unordered_set<Node*> o1Set;
      Node* cur = o1;
      o1Set.emplace(cur);
      while (parentMap.at(cur) != nullptr)  // 把 o1 及所有的父节点加入o1Set
      {
        cur = parentMap.at(cur);
        o1Set.emplace(cur);
      }
      cur = o2;
      while (o1Set.find(cur) == o1Set.end())  // 找到一个最近的公共祖先节点
      {
        cur = parentMap.at(cur);
      }
      Node* lowestAncestor = cur;
      cur = o1;
      int distance1 = 1;
      while (cur != lowestAncestor)  // 计算o1到最近公共祖先节点的距离
      {
        cur = parentMap.at(cur);
        distance1++;
      }
      cur = o2;
      int distance2 = 1;
      while (cur != lowestAncestor)  // 计算o2到最近公共祖先节点的距离
      {
        cur = parentMap.at(cur);
        distance2++;
      }
      return distance1 + distance2 - 1;
    }

    static int maxDistance2(Node* head)
    {
      return process(head).maxDistance;
    }

    class Info
    {
      public:
        int maxDistance;
        int height;

        Info(int m, int h)
        {
          maxDistance = m;
          height = h;
        }
    };

    static Info process(Node* x)
    {
      if (x == nullptr)
      {
        return Info(0, 0);
      }
      Info leftInfo = process(x->left);
      Info rightInfo = process(x->right);
      int height = std::max(leftInfo.height, rightInfo.height) + 1;
      //不经过X：需要获取左子树和右子树各自的最大距离
      //经过X：（左树高度 + 右树高度 + 1）即是最大距离
      int p1 = leftInfo.maxDistance;
      int p2 = rightInfo.maxDistance;
      int p3 = leftInfo.height + rightInfo.height + 1;
      int maxDistance = std::max(std::max(p1, p2), p3);
      return Info(maxDistance, height);
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
        if (maxDistance1(head) != maxDistance2(head))
        {
          cout << "Oops!" << endl;
        }
      }
      cout << "finish!" << endl;
    }
};

int main()
{
  MaxDistance::test();
  return 0;
}
