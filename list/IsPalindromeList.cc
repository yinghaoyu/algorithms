#include <stdbool.h>
#include <iostream>
#include <vector>

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
// 2、额外空间复杂度为O(N)的写法：
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
    Node *next;

    Node(int data) { this->value = data; }
  };

  // need n extra space
  static bool isPalindrome1(Node *head)
  {
    vector<Node *> stack;
    Node *cur = head;
    while (cur != nullptr)  // 全部元素进栈，额外空间O(n)
    {
      stack.push_back(cur);
      cur = cur->next;
    }
    while (head != nullptr)  // 逆序比较
    {
      Node *tmp = stack.back();
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
  static bool isPalindrome2(Node *head)
  {
    if (head == nullptr || head->next == nullptr)
    {
      return true;
    }
    Node *right = head->next;
    Node *cur = head;
    while (cur->next != nullptr && cur->next->next != nullptr)  // 快慢指针
    {
      right = right->next;
      cur = cur->next->next;
    }
    vector<Node *> stack;
    while (right != nullptr)  // 后半部分进栈
    {
      stack.push_back(right);
      right = right->next;
    }
    while (!stack.empty())
    {
      Node *tmp = stack.back();
      stack.pop_back();
      if (head->value != tmp->value)
      {
        return false;
      }
      head = head->next;
    }
    return true;
  }

  static Node *reverse(Node *head)
  {
    Node *pre = NULL;
    Node *next = NULL;
    while (head != NULL)
    {
      next = head->next;
      head->next = pre;
      pre = head;
      head = next;
    }
    return pre;
  }

  // need O(1) extra space
  static bool isPalindrome3(Node *head)
  {
    if (head == NULL || head->next == NULL)
    {
      return true;
    }
    Node *slow = head;
    Node *fast = head;
    Node *tmp = NULL;
    while (fast->next != NULL && fast->next->next != NULL)
    {
      slow = slow->next;
      fast = fast->next->next;
    }
    // 奇数移动到中点，偶数移动到下中点
    tmp = slow->next;
    // 左右两边断开链表
    slow->next = NULL;
    Node *r = reverse(tmp);
    tmp = r;
    bool res = true;
    while (head != NULL && tmp != NULL)
    {
      if (head->value != tmp->value)
      {
        res = false;
        break;
      }
      head = head->next;
      tmp = tmp->next;
    }
    r = reverse(r);
    // 恢复链表
    slow->next = r;
    return res;
  }

  static void printLinkedList(Node *node)
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
  IsPalindromeList::Node *head = nullptr;
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
