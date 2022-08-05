#include <iostream>
#include <random>
#include <vector>

using namespace std;

// 问题描述：
// 对于一串无环单链表
// 奇数找到中点，偶数找到上中点或者下中点
// 奇数找到中点前一个，偶数找到上中点前一个或者下中点前一个

class LinkedListMid
{
 public:
  class Node
  {
   public:
    int value;
    Node *next;

    Node(int v) { value = v; }
  };

  // 奇数返回中点，偶数返回上中点
  static Node *midOrUpMidNode(Node *head)
  {
    if (head == nullptr || head->next == nullptr || head->next->next == nullptr)
    {
      return head;
    }
    // 链表有3个点或以上
    Node *slow = head->next;
    Node *fast = head->next->next;
    while (fast->next != nullptr && fast->next->next != nullptr)
    {
      slow = slow->next;
      fast = fast->next->next;
    }
    return slow;
  }

  // 奇数返回中点，偶数返回下中点
  static Node *midOrDownMidNode(Node *head)
  {
    if (head == nullptr || head->next == nullptr)
    {
      return head;
    }
    Node *slow = head->next;
    Node *fast = head->next;  // 起始快指针少走一步
    while (fast->next != nullptr && fast->next->next != nullptr)
    {
      slow = slow->next;
      fast = fast->next->next;
    }
    return slow;
  }

  // 奇数返回中点前一个，偶数返回上中点前一个
  static Node *midOrUpMidPreNode(Node *head)
  {
    if (head == nullptr || head->next == nullptr || head->next->next == nullptr)
    {
      return nullptr;
    }
    Node *slow = head;
    Node *fast = head->next->next;
    while (fast->next != nullptr && fast->next->next != nullptr)
    {
      slow = slow->next;
      fast = fast->next->next;
    }
    return slow;
  }

  // 奇数返回中点前一个，偶数返回下中点前一个
  static Node *midOrDownMidPreNode(Node *head)
  {
    if (head == nullptr || head->next == nullptr)
    {
      return nullptr;
    }

    if (head->next->next == nullptr)
    {
      return head;
    }
    Node *slow = head;
    Node *fast = head->next;
    while (fast->next != nullptr && fast->next->next != nullptr)
    {
      slow = slow->next;
      fast = fast->next->next;
    }
    return slow;
  }

  // 下面的比上面的coding更简单，但是空间复杂度较大
  // 奇数返回中点，偶数返回上中点
  static Node *right1(Node *head)
  {
    if (head == nullptr)
    {
      return nullptr;
    }
    Node *cur = head;
    vector<Node *> arr;
    while (cur != nullptr)
    {
      arr.push_back(cur);
      cur = cur->next;
    }
    return arr.at((arr.size() - 1) / 2);
  }

  // 奇数返回中点，偶数返回下中点
  static Node *right2(Node *head)
  {
    if (head == nullptr)
    {
      return nullptr;
    }
    Node *cur = head;
    vector<Node *> arr;
    while (cur != nullptr)
    {
      arr.push_back(cur);
      cur = cur->next;
    }
    return arr.at(arr.size() / 2);
  }

  //奇数返回中点前一个，偶数返回上中点前一个
  static Node *right3(Node *head)
  {
    if (head == nullptr || head->next == nullptr || head->next->next == nullptr)
    {
      return nullptr;
    }
    Node *cur = head;
    vector<Node *> arr;
    while (cur != nullptr)
    {
      arr.push_back(cur);
      cur = cur->next;
    }
    return arr.at((arr.size() - 3) / 2);
  }

  // 奇数返回中点前一个，偶数返回下中点前一个
  static Node *right4(Node *head)
  {
    if (head == nullptr || head->next == nullptr)
    {
      return nullptr;
    }
    Node *cur = head;
    vector<Node *> arr;
    while (cur != nullptr)
    {
      arr.push_back(cur);
      cur = cur->next;
    }
    return arr.at((arr.size() - 2) / 2);
  }

  static void test()
  {
    Node *test = nullptr;
    test = new Node(0);
    test->next = new Node(1);
    test->next->next = new Node(2);
    test->next->next->next = new Node(3);
    test->next->next->next->next = new Node(4);
    test->next->next->next->next->next = new Node(5);
    test->next->next->next->next->next->next = new Node(6);
    test->next->next->next->next->next->next->next = new Node(7);
    test->next->next->next->next->next->next->next->next = new Node(8);

    Node *ans1 = nullptr;
    Node *ans2 = nullptr;

    ans1 = midOrUpMidNode(test);
    ans2 = right1(test);
    cout << (ans1 != nullptr ? std::to_string(ans1->value) : "无") << endl;
    cout << (ans2 != nullptr ? std::to_string(ans2->value) : "无") << endl;

    ans1 = midOrDownMidNode(test);
    ans2 = right2(test);
    cout << (ans1 != nullptr ? std::to_string(ans1->value) : "无") << endl;
    cout << (ans2 != nullptr ? std::to_string(ans2->value) : "无") << endl;
    ans1 = midOrUpMidPreNode(test);
    ans2 = right3(test);
    cout << (ans1 != nullptr ? std::to_string(ans1->value) : "无") << endl;
    cout << (ans2 != nullptr ? std::to_string(ans2->value) : "无") << endl;

    ans1 = midOrDownMidPreNode(test);
    ans2 = right4(test);
    cout << (ans1 != nullptr ? std::to_string(ans1->value) : "无") << endl;
    cout << (ans2 != nullptr ? std::to_string(ans2->value) : "无") << endl;
  }
};

int main()
{
  LinkedListMid::test();
  return 0;
}
