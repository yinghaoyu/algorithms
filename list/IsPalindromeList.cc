#include <iostream>
#include <vector>
#include <stdbool.h>

using namespace std;

// 判断链表是不是回文结构
// 例如：1 --> 2 --> 2 --> 1，1 --> 2 --> 3 --> 2 --> 1
//
// 1、额外空间复杂度为O(1)的写法
//（1）找到此链表的中点（偶数个节点则找上中点）
//（2）调整链表结构
//（3）判断是否为回文结构
//（4）将链表调整为原来的结构
//（5）返回结果
//
//2、额外空间复杂度为O(N)的写法：
//（1）将链表元素放进栈中，因为栈先进后出，等于把链表翻转了
//（2）从原链表头节点开始，挨个和栈弹出的元素进行比较
//（3）每次的值都相等，则是回文结构

class IsPalindromeList
{
  public:
    class Node
    {
      public:
        int value;
        Node* next;

        Node(int data)
        {
          this->value = data;
        }
    };

    // need n extra space
    static bool isPalindrome1(Node* head)
    {
      vector<Node*> stack;
      Node* cur = head;
      while (cur != nullptr)  // 全部元素进栈，额外空间O(n)
      {
        stack.push_back(cur);
        cur = cur->next;
      }
      while (head != nullptr)  // 逆序比较
      {
        Node* tmp = stack.back();
        stack.pop_back();
        if (head->value != tmp->value)
        {
          return false;
        }
        head = head->next;
      }
      return true;
    }

    // need n/2 extra space
    static bool isPalindrome2(Node* head)
    {
      if (head == nullptr || head->next == nullptr)
      {
        return true;
      }
      Node* right = head->next;
      Node* cur = head;
      while (cur->next != nullptr && cur->next->next != nullptr)  // 快慢指针
      {
        right = right->next;
        cur = cur->next->next;
      }
      vector<Node*> stack;
      while (right != nullptr)  // 后半部分进栈
      {
        stack.push_back(right);
        right = right->next;
      }
      while (!stack.empty())
      {
        Node* tmp = stack.back();
        stack.pop_back();
        if (head->value != tmp->value)
        {
          return false;
        }
        head = head->next;
      }
      return true;
    }

    // need O(1) extra space
    static bool isPalindrome3(Node* head)
    {
      if (head == nullptr || head->next == nullptr)
      {
        return true;
      }

      Node* n1 = head;
      Node* n2 = head;
      while (n2->next != nullptr && n2->next->next != nullptr)
      {
        // find mid node
        n1 = n1->next; // n1 -> mid
        n2 = n2->next->next; // n2 -> end
      }
      // n1 中点 1 2 3 2 1被分为1 --> 2 --> 3和3 <-- 2 <-- 1
      n2 = n1->next; // n2 -> right part first node
      n1->next = nullptr; // mid.next -> nullptr 左右链表截断
      Node* n3 = nullptr;
      while (n2 != nullptr)
      {
        // right part convert
        n3 = n2->next; // n3 -> save next node
        n2->next = n1; // next of right node convert
        n1 = n2; // n1 move
        n2 = n3; // n2 move
      }
      n3 = n1; // n3 -> save last node
      n2 = head;// n2 -> left first node
      bool res = true;
      while (n1 != nullptr && n2 != nullptr)
      {
        // check palindrome
        if (n1->value != n2->value)
        {
          res = false;
          break;
        }
        n1 = n1->next; // left to mid
        n2 = n2->next; // right to mid
      }
      // recover
      n1 = n3->next;
      n3->next = nullptr;  // last node point to nullptr
      while (n1 != nullptr)
      {
        // recover list
        n2 = n1->next;
        n1->next = n3;
        n3 = n1;
        n1 = n2;
      }
      return res;
    }

    static void printLinkedList(Node* node)
    {
      cout << "Linked List: ";
      while (node != nullptr)
      {
        cout << node->value << " ";
        node = node->next;
      }
      cout << endl;
    }
};

int main()
{
  IsPalindromeList::Node* head = nullptr;
  IsPalindromeList::IsPalindromeList::printLinkedList(head);
  cout << IsPalindromeList::isPalindrome1(head) << " | ";
  cout << IsPalindromeList::isPalindrome2(head) << " | ";
  cout << IsPalindromeList::isPalindrome3(head) << " | " << endl;
  IsPalindromeList::printLinkedList(head);
  cout << "=========================" << endl;

  head = new IsPalindromeList::Node(1);
  IsPalindromeList::printLinkedList(head);
  cout << IsPalindromeList::isPalindrome1(head) << " | ";
  cout << IsPalindromeList::isPalindrome2(head) << " | ";
  cout << IsPalindromeList::isPalindrome3(head) << " | " << endl;
  IsPalindromeList::printLinkedList(head);
  cout << "=========================" << endl;

  head = new IsPalindromeList::Node(1);
  head->next = new IsPalindromeList::Node(2);
  IsPalindromeList::printLinkedList(head);
  cout << IsPalindromeList::isPalindrome1(head) << " | ";
  cout << IsPalindromeList::isPalindrome2(head) << " | ";
  cout << IsPalindromeList::isPalindrome3(head) << " | " << endl;
  IsPalindromeList::printLinkedList(head);
  cout << "=========================" << endl;

  head = new IsPalindromeList::Node(1);
  head->next = new IsPalindromeList::Node(1);
  IsPalindromeList::printLinkedList(head);
  cout << IsPalindromeList::isPalindrome1(head) << " | ";
  cout << IsPalindromeList::isPalindrome2(head) << " | ";
  cout << IsPalindromeList::isPalindrome3(head) << " | " << endl;
  IsPalindromeList::printLinkedList(head);
  cout << "=========================" << endl;

  head = new IsPalindromeList::Node(1);
  head->next = new IsPalindromeList::Node(2);
  head->next->next = new IsPalindromeList::Node(3);
  IsPalindromeList::printLinkedList(head);
  cout << IsPalindromeList::isPalindrome1(head) << " | ";
  cout << IsPalindromeList::isPalindrome2(head) << " | ";
  cout << IsPalindromeList::isPalindrome3(head) << " | " << endl;
  IsPalindromeList::printLinkedList(head);
  cout << "=========================" << endl;

  head = new IsPalindromeList::Node(1);
  head->next = new IsPalindromeList::Node(2);
  head->next->next = new IsPalindromeList::Node(1);
  IsPalindromeList::printLinkedList(head);
  cout << IsPalindromeList::isPalindrome1(head) << " | ";
  cout << IsPalindromeList::isPalindrome2(head) << " | ";
  cout << IsPalindromeList::isPalindrome3(head) << " | " << endl;
  IsPalindromeList::printLinkedList(head);
  cout << "=========================" << endl;

  head = new IsPalindromeList::Node(1);
  head->next = new IsPalindromeList::Node(2);
  head->next->next = new IsPalindromeList::Node(3);
  head->next->next->next = new IsPalindromeList::Node(1);
  IsPalindromeList::printLinkedList(head);
  cout << IsPalindromeList::isPalindrome1(head) << " | ";
  cout << IsPalindromeList::isPalindrome2(head) << " | ";
  cout << IsPalindromeList::isPalindrome3(head) << " | " << endl;
  IsPalindromeList::printLinkedList(head);
  cout << "=========================" << endl;

  head = new IsPalindromeList::Node(1);
  head->next = new IsPalindromeList::Node(2);
  head->next->next = new IsPalindromeList::Node(2);
  head->next->next->next = new IsPalindromeList::Node(1);
  IsPalindromeList::printLinkedList(head);
  cout << IsPalindromeList::isPalindrome1(head) << " | ";
  cout << IsPalindromeList::isPalindrome2(head) << " | ";
  cout << IsPalindromeList::isPalindrome3(head) << " | " << endl;
  IsPalindromeList::printLinkedList(head);
  cout << "=========================" << endl;

  head = new IsPalindromeList::Node(1);
  head->next = new IsPalindromeList::Node(2);
  head->next->next = new IsPalindromeList::Node(3);
  head->next->next->next = new IsPalindromeList::Node(2);
  head->next->next->next->next = new IsPalindromeList::Node(1);
  IsPalindromeList::printLinkedList(head);
  cout << IsPalindromeList::isPalindrome1(head) << " | ";
  cout << IsPalindromeList::isPalindrome2(head) << " | ";
  cout << IsPalindromeList::isPalindrome3(head) << " | " << endl;
  IsPalindromeList::printLinkedList(head);
  cout << "=========================" << endl;
  return 0;
}
