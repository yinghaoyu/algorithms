#include <iostream>
#include <list>

using namespace std;

// 将n叉树编码为二叉树
//     1              1
//   ↙ ↓ ↘           ↙
//  3  2  4   -->   3
// ↙ ↘             ↙ ↘
// 5  6           5   2
//                 ↘   ↘
//                  6   4

// https://leetcode.cn/problems/encode-n-ary-tree-to-binary-tree/

class EncodeNaryTreeToBinaryTree
{
  public:
    class Node
    {
      public:
        int val;
        list<Node*> children;  // 多叉树的孩子

        Node()
        {
        }

        Node(int _val)
        {
          val = _val;
        }

        Node(int _val, list<Node*> _children)
        {
          val = _val;
          children = _children;
        }
    };

    class TreeNode
    {
      public:
        int val;
        TreeNode* left;
        TreeNode* right;

        TreeNode(int x)
        {
          val = x;
        }
    };

    class Codec
    {
      // Encodes an n-ary tree to a binary tree.
      public:
        TreeNode* encode(Node* root)
        {
          if (root == nullptr)
          {
            return nullptr;
          }
          TreeNode* head = new TreeNode(root->val);
          head->left = en(root->children);
          return head;
        }

        // 将多叉树的孩子编码为二叉树，返回二叉树的根节点
        TreeNode* en(list<Node*>& children)
        {
          TreeNode* head = nullptr;
          TreeNode* cur = nullptr;
          for (Node* child : children)
          {
            TreeNode* tNode = new TreeNode(child->val);
            if (head == nullptr)
            {
              head = tNode;
            }
            else
            {
              cur->right = tNode;  // 兄弟孩子依次变成右节点
            }
            cur = tNode;
            cur->left = en(child->children);  // 左孩子重新递归编码
          }
          return head;
        }

        // Decodes your binary tree to an n-ary tree.
        Node* decode(TreeNode* root)
        {
          if (root == nullptr)
          {
            return nullptr;
          }
          return new Node(root->val, de(root->left));
        }

        list<Node*> de(TreeNode* root)
        {
          list<Node*> children;
          while (root != nullptr)
          {
            Node* cur = new Node(root->val, de(root->left));
            children.push_back(cur);
            root = root->right;  // 左孩子的所有右节点都是孩子节点
          }
          return children;
        }
    };
};
