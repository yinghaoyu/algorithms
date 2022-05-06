#include <iostream>
#include <stdbool.h>
#include <vector>
#include <queue>
#include <unordered_map>
#include <random>

#define MAX_SEED 1024
#define HALF_SEED (MAX_SEED >> 1)

using namespace std;


//求二叉树最宽的层有多少个节点
//
//思路：在按层遍历的基础上进行改写。
//（1）准备currentLevelEnd（表示当前层的结束节点）、nextLevelEnd（表示下一层的结束节点）、currentWeight（表示当前层的宽度）、maxWeight（表示最大宽度）四个变量。
//（2）currentLevelEnd修改为二叉树的头节点（其他均为默认值），头节点入队列。
//（3）从队列中弹出一个节点，每出队一个节点则currentWeight加一，出队节点有左孩子则左孩子入队，有右孩子则右孩子入队，有节点入队则将nextLevelEnd修改为当前入队的节点（为下一层查找做准备）。
//（4）判断第3步出队的节点是否等于currentLevelEnd。
//      是，则修改maxWeight为当前最大宽度，currentWeight重置，currentLevelEnd修改为nextLevelEnd，nextLevelEnd重置；
//      否，则继续执行第3步。
//（5）一直执行第3、4步，直到队列为空。

class TreeMaxWidth
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

    static int getRandom(int min, int max)
    {
      random_device seed;  // 硬件生成随机数种子
      ranlux48 engine(seed());  // 利用种子生成随机数引
      uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
      int res = distrib(engine);  // 随机数
      return res;
    }

    static int maxWidthUseMap(Node* head)
    {
      if (head == NULL)
      {
        return 0;
      }
      queue<Node*> queue;
      queue.push(head);
      // key 在 哪一层，value
      unordered_map<Node*, int> levelMap;
      levelMap.emplace(head, 1);
      int curLevel = 1; // 当前你正在统计哪一层的宽度
      int curLevelNodes = 0; // 当前层curLevel层，宽度目前是多少
      int max = 0;
      while (!queue.empty())
      {
        Node* cur = queue.front();
        queue.pop();
        int curNodeLevel = levelMap.at(cur);
        if (cur->left != NULL)
        {
          levelMap.emplace(cur->left, curNodeLevel + 1);
          queue.push(cur->left);
        }
        if (cur->right != NULL)
        {
          levelMap.emplace(cur->right, curNodeLevel + 1);
          queue.push(cur->right);
        }
        if (curNodeLevel == curLevel)
        {
          curLevelNodes++;
        }
        else
        {
          max = std::max(max, curLevelNodes);
          curLevel++;
          curLevelNodes = 1;
        }
      }
      max = std::max(max, curLevelNodes);
      return max;
    }

    static int maxWidthNoMap(Node* head)
    {
      if (head == NULL)
      {
        return 0;
      }
      queue<Node*> queue;
      queue.push(head);
      Node* curEnd = head; // 当前层，最右节点是谁
      Node* nextEnd = NULL; // 下一层，最右节点是谁
      int max = 0;
      int curLevelNodes = 0; // 当前层的节点数
      while (!queue.empty())
      {
        Node* cur = queue.front();
        queue.pop();
        if (cur->left != NULL)
        {
          queue.emplace(cur->left);
          nextEnd = cur->left;
        }
        if (cur->right != NULL)
        {
          queue.emplace(cur->right);
          nextEnd = cur->right;
        }
        curLevelNodes++;
        if (cur == curEnd)
        {
          max = std::max(max, curLevelNodes);
          curLevelNodes = 0;
          curEnd = nextEnd;
        }
      }
      return max;
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
      int maxLevel = 10;
      int maxValue = 100;
      int testTimes = 10000;
      for (int i = 0; i < testTimes; i++)
      {
        Node* head = generateRandomBST(maxLevel, maxValue);
        if (maxWidthUseMap(head) != maxWidthNoMap(head))
        {
          cout << "Oops!" << endl;
        }
      }
      cout << "finish!" << endl;
    }
};

int main()
{
  TreeMaxWidth::test();
  return 0;
}
