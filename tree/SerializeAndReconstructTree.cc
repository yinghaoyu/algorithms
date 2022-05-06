#include <iostream>
#include <string>
#include <random>
#include <queue>
#include <vector>
#include <stdbool.h>

#define MAX_SEED 1024
#define HALF_SEED (MAX_SEED >> 1)

using namespace std;

// 序列化和反序列化二叉树

class SerializeAndReconstructTree
{
  /*
   * 二叉树可以通过先序、后序或者按层遍历的方式序列化和反序列化，
   * 以下代码全部实现了。
   * 但是，二叉树无法通过中序遍历的方式实现序列化和反序列化
   * 因为不同的两棵树，可能得到同样的中序序列，即便补了空位置也可能一样。
   * 比如如下两棵树
   *         __2
   *        /
   *       1
   *       和
   *       1__
   *          \
   *           2
   * 补足空位置的中序遍历结果都是{ NULL, 1, NULL, 2, NULL}
   *
   * */
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

  static queue<string> preSerial(Node* head)
  {
    queue<string> ans;
    pres(head, ans);
    return ans;
  }

  // 先序序列化
  static void pres(Node* head, queue<string>& ans)
  {
    if (head == NULL)
    {
      ans.push("NULL");
    }
    else
    {
      ans.push(std::to_string(head->value));
      pres(head->left, ans);
      pres(head->right, ans);
    }
  }

  static queue<string> inSerial(Node* head)
  {
    queue<string> ans;
    ins(head, ans);
    return ans;
  }

  // 中序序列化，生成的树不唯一
  static void ins(Node* head, queue<string>& ans)
  {
    if (head == NULL)
    {
      ans.push("NULL");
    }
    else
    {
      ins(head->left, ans);
      ans.push(std::to_string(head->value));
      ins(head->right, ans);
    }
  }

  static queue<string> posSerial(Node* head)
  {
    queue<string> ans;
    poss(head, ans);
    return ans;
  }

  // 后序序列化
  static void poss(Node* head, queue<string>& ans)
  {
    if (head == NULL)
    {
      ans.push("NULL");
    }
    else
    {
      poss(head->left, ans);
      poss(head->right, ans);
      ans.push(std::to_string(head->value));
    }
  }

  static Node* buildByPrequeue(queue<string>& prelist)
  {
    if (prelist.size() == 0)
    {
      return NULL;
    }
    return preb(prelist);
  }

  // 先序反序列化
  static Node* preb(queue<string>& prelist)
  {
    string value = prelist.front();
    prelist.pop();
    if (value == "NULL")
    {
      return NULL;
    }
    Node* head = new Node(atoi(value.c_str()));
    head->left = preb(prelist);
    head->right = preb(prelist);
    return head;
  }

  // 需要注意的是在反序列化时，
  // 得先把原始序列化的顺序颠倒一下（颠倒后的顺序即是根右左）
  // 再以根右左的方式进行反序列化，因为要先有头才有孩子（先构造头才能构造孩子），这样反序列化的结果才是对的
  static Node* buildByPosqueue(queue<string>& poslist)
  {
    if (poslist.size() == 0)
    {
      return NULL;
    }

    // 左右中  ->  stack(中右左)
    vector<string> stack;
    while (!poslist.empty())
    {
      stack.push_back(poslist.front());
      poslist.pop();
    }
    return posb(stack);
  }

  // 后序反序列化
  static Node* posb(vector<string>& posstack)
  {
    string value = posstack.back();
    posstack.pop_back();
    if (value == "NULL")
    {
      return NULL;
    }
    Node* head = new Node(atoi(value.c_str()));
    head->right = posb(posstack);
    head->left = posb(posstack);
    return head;
  }

  // 层序序列化
  static queue<string> levelSerial(Node* head)
  {
    queue<string> ans;
    if (head == NULL)
    {
      ans.push("NULL");
    }
    else
    {
      ans.push(std::to_string(head->value));
      queue<Node*> queue;
      queue.push(head);
      while (!queue.empty())
      {
        head = queue.front(); // head 父   子
        queue.pop();
        if (head->left != NULL)
        {
          ans.push(std::to_string(head->left->value));
          queue.push(head->left);
        }
        else
        {
          ans.push("NULL");  // 无左孩子要补空
        }
        if (head->right != NULL)
        {
          ans.push(std::to_string(head->right->value));
          queue.push(head->right);
        }
        else
        {
          ans.push("NULL");  // 无右孩子要补空
        }
      }
    }
    return ans;
  }

  // 层序反序列化
  static Node* buildByLevelqueue(queue<string>& levelList)
  {
    if (levelList.size() == 0)
    {
      return NULL;
    }
    Node* head = generateNode(levelList.front());
    levelList.pop();
    queue<Node*> queue;
    if (head != NULL)
    {
      queue.push(head);
    }
    Node* node = NULL;
    while (!queue.empty())
    {
      node = queue.front();
      queue.pop();
      node->left = generateNode(levelList.front());
      levelList.pop();
      node->right = generateNode(levelList.front());
      levelList.pop();
      if (node->left != NULL)
      {
        queue.push(node->left);
      }
      if (node->right != NULL)
      {
        queue.push(node->right);
      }
    }
    return head;
  }

  static Node* generateNode(string val)
  {
    if (val == "NULL")
    {
      return NULL;
    }
    return new Node(atoi(val.c_str()));
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

  // for test
  static bool isSameValueStructure(Node* head1, Node* head2)
  {
    if (head1 == NULL && head2 != NULL)
    {
      return false;
    }
    if (head1 != NULL && head2 == NULL)
    {
      return false;
    }
    if (head1 == NULL && head2 == NULL)
    {
      return true;
    }
    if (head1->value != head2->value)
    {
      return false;
    }
    return isSameValueStructure(head1->left, head2->left) &&
            isSameValueStructure(head1->right, head2->right);
  }

  // for test
  static void printTree(Node* head)
  {
    cout << "Binary Tree:" << endl;
    printInOrder(head, 0, "H", 17);
    cout << endl;
  }

  static void printInOrder(Node* head, int height, string to, int len)
  {
    if (head == NULL)
    {
      return;
    }
    printInOrder(head->right, height + 1, "v", len);
    string val = to + std::to_string(head->value) + to;  // to 用来标记节点
    int lenM = val.length();
    int lenL = (len - lenM) / 2;  // 把val格式化成len的长度，居中对齐，左边需要的空格数
    int lenR = len - lenM - lenL;  // 右边需要的空格数
    val = getSpace(lenL) + val + getSpace(lenR);  // 格式化成len长度的字符串
    cout << getSpace(height * len) + val << endl;  // 高度*len个空格 + 带值的len
    printInOrder(head->left, height + 1, "^", len);
  }

  static string getSpace(int num)
  {
    return string(num, ' ');
  }

  static void test()
  {
    int maxLevel = 5;
    int maxValue = 100;
    int testTimes = 10000;
    cout << "test begin" << endl;
    for (int i = 0; i < testTimes; i++)
    {
      Node* head = generateRandomBST(maxLevel, maxValue);
      queue<string> pre = preSerial(head);
      queue<string> pos = posSerial(head);
      queue<string> level = levelSerial(head);
      Node* preBuild = buildByPrequeue(pre);
      Node* posBuild = buildByPosqueue(pos);
      Node* levelBuild = buildByLevelqueue(level);
      if (!isSameValueStructure(preBuild, posBuild) ||
          !isSameValueStructure(posBuild, levelBuild  ))
      {
        cout << "Oops!" << endl;
      }
    }
    cout << "test finish!" << endl;
  }
};

int main()
{
  SerializeAndReconstructTree::test();
  return 0;
}
