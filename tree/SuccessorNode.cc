#include <iostream>
#include <stdbool.h>

using namespace std;

// 查询后继节点
// 思路：
// 1、按照后继节点的定义来查找某个节点的后继节点
// 即按中序遍历此二叉树，再查找指定节点的后继节点。时间复杂度是O(N)。显然此方法不是最优解。
//
// 2、根据二叉树的结构，以及多的这个parent指针来查找
//  （1）指定节点X有右树，则X的后继节点就是X右树上最左的孩子。根据后继节点的定义得出的。
//  （2）X无右树，则根据parent指针往上找，直到找到某个节点是它父节点Y的左孩子为止，则Y就是X的后继节点；如果一直往上都没有找到某个节点是它父节点的左孩子，则该节点是最右的孩子，无后继节点。
// Why：因为X是Y左树上的最右孩子，按照中序遍历的顺序，遍历完X就是Y了。
//   情况一      |   情况二    |    情况三
//       Y       |        .    | .
//     ↗         |       ↗     |  ↖
//    .          |      .      |   .
//     ↖         |     ↗       |    ↖
//      .        |    .        |     .
//       ↖       |   ↗         |      ↖
//        .      |  Y          |       .
//         ↖     | ↗           |        ↖
//          X    |X            |         X
// 时间复杂度是O(K)（K是指定节点到后继节的最短节点数）

class SuccessorNode
{
  public:
    class Node
    {
      public:
        int value;
        Node* left;
        Node* right;
        Node* parent;

        Node(int data)
        {
          this->value = data;
        }
    };

    static Node* getSuccessorNode(Node* node)
    {
      if (node == NULL)
      {
        return node;
      }
      if (node->right != NULL)
      {
        return getLeftMost(node->right);
      }
      else
      {
        // 无右子树
        Node* parent = node->parent;
        while (parent != NULL && parent->right == node)
        {
          // 当前节点是其父亲节点右孩子
          node = parent;
          parent = node->parent;
        }
        return parent;
      }
    }

    static Node* getLeftMost(Node* node)
    {
      if (node == NULL)
      {
        return node;
      }
      while (node->left != NULL)
      {
        node = node->left;
      }
      return node;
    }

    static void test()
    {
      Node* head = new Node(6);
      head->parent = NULL;
      head->left = new Node(3);
      head->left->parent = head;
      head->left->left = new Node(1);
      head->left->left->parent = head->left;
      head->left->left->right = new Node(2);
      head->left->left->right->parent = head->left->left;
      head->left->right = new Node(4);
      head->left->right->parent = head->left;
      head->left->right->right = new Node(5);
      head->left->right->right->parent = head->left->right;
      head->right = new Node(9);
      head->right->parent = head;
      head->right->left = new Node(8);
      head->right->left->parent = head->right;
      head->right->left->left = new Node(7);
      head->right->left->left->parent = head->right->left;
      head->right->right = new Node(10);
      head->right->right->parent = head->right;
      Node* test = head->left->left;
      cout << test->value << " next: " << getSuccessorNode(test)->value <<endl;
      test = head->left->left->right;
      cout << test->value << " next: " << getSuccessorNode(test)->value <<endl;
      test = head->left;
      cout << test->value << " next: " << getSuccessorNode(test)->value <<endl;
      test = head->left->right;
      cout << test->value << " next: " << getSuccessorNode(test)->value <<endl;
      test = head->left->right->right;
      cout << test->value << " next: " << getSuccessorNode(test)->value <<endl;
      test = head;
      cout << test->value << " next: " << getSuccessorNode(test)->value <<endl;
      test = head->right->left->left;
      cout << test->value << " next: " << getSuccessorNode(test)->value <<endl;
      test = head->right->left;
      cout << test->value << " next: " << getSuccessorNode(test)->value <<endl;
      test = head->right;
      cout << test->value << " next: " << getSuccessorNode(test)->value <<endl;
      test = head->right->right; // 10's next is NULL
      cout << test->value << " next: " << getSuccessorNode(test) <<endl;
    }
};

int main()
{
  SuccessorNode::test();
  return 0;
}
