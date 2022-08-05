#include <stdbool.h>
#include <iostream>
#include <unordered_map>

using namespace std;

// 测试链接 : https://leetcode.com/problems/copy-list-with-random-pointer/
//
// 给你一个长度为 n 的链表，每个节点包含一个额外增加的随机指针 random ，该指针可以指向链表中的任何节点或空节点。
// 构造这个链表的深拷贝。深拷贝应该正好由 n 个 全新节点组成，其中每个新节点的值都设为其对应的原节点的值。新节点的 next 指针和 random
// 指针也都应指向复制链表中的新节点，并使原链表和复制链表中的这些指针能够表示相同的链表状态。复制链表中的指针都不应指向原链表中的节点 。

// 例如，如果原链表中有 X 和 Y 两个节点，其中 X.random --> Y 。那么在复制链表中对应的两个节点 x 和 y ，同样有 x.random --> y 。

// 返回复制链表的头节点。

// 一、额外空间复杂度为O(1)的写法
// 1、在原链表每个节点后面，复制一个新的节点，并连到老节点后面
// 即：
// 复制前：1 -> 2 -> 3，
// 复制后：1 -> 1` -> 2 -> 2` -> 3 -> 3`
// 2、将原节点random指针的关系，在新节点中复制一份
// 3、分离新老节点
//
// 二、额外空间复杂度为O(N)的写法
// 1、使用Map存储新老节点，Key：老节点，Value：新节点
// 2、构造老节点对应的新节点
// 3、调整新节点的next指针、调整新节点的random指针

class CopyListWithRandom
{
 public:
  class Node
  {
   public:
    int val;
    Node *next;
    Node *random;

    Node(int val)
    {
      this->val = val;
      this->next = nullptr;
      this->random = nullptr;
    }
  };

  static Node *copyRandomList1(Node *head)
  {
    // key 老节点
    // value 新节点
    unordered_map<Node *, Node *> map;
    Node *cur = head;
    while (cur != nullptr)
    {
      // 构造老节点对应的新节点
      map.emplace(cur, new Node(cur->val));
      cur = cur->next;
    }
    cur = head;
    while (cur != nullptr)
    {
      // cur 老
      // map.at(cur) 新
      // 新.next ->  cur.next克隆节点找到
      map.at(cur)->next = map.at(cur->next);
      map.at(cur)->random = map.at(cur->random);
      cur = cur->next;
    }
    return map.at(head);
  }

  static Node *copyRandomList2(Node *head)
  {
    if (head == nullptr)
    {
      return nullptr;
    }
    Node *cur = head;
    Node *next = nullptr;
    // 1 -> 2 -> 3 -> nullptr
    // 1 -> 1' -> 2 -> 2' -> 3 -> 3'
    while (cur != nullptr)
    {
      next = cur->next;
      cur->next = new Node(cur->val);
      cur->next->next = next;
      cur = next;
    }
    cur = head;
    Node *copy = nullptr;
    // 1 1' 2 2' 3 3'
    // 依次设置 1' 2' 3' random指针
    while (cur != nullptr)
    {
      next = cur->next->next;
      copy = cur->next;
      copy->random = cur->random != nullptr ? cur->random->next : nullptr;
      cur = next;
    }
    Node *res = head->next;
    cur = head;
    // 老 新 混在一起，next方向上，random正确
    // next方向上，把新老链表分离
    while (cur != nullptr)
    {
      next = cur->next->next;
      copy = cur->next;
      cur->next = next;
      copy->next = next != nullptr ? next->next : nullptr;
      cur = next;
    }
    return res;
  }
};
