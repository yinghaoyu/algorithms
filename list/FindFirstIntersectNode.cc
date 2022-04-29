#include <iostream>
#include <stdlib.h>
#include <random>

using namespace std;

// 给定两个可能有环也可能无环的单链表，头节点head1和head2。
// 请实现一个函数，如果两个链表相交，请返回相交的第一个节点。如果不相交，返回NULL
// 要求：如果两个链表长度之和为N，时间复杂度请达到O(N)，额外空间复杂度请达到O(1)。
// 思路：
// 先找单链表的入环节点。从头结点开始，快指针走一步，慢指针走两步。如果有环必然相遇。
// 1、两个链表都无环，记为 Y 结构。
//      ↘       ↙
//        ↘   ↙
//          ↓
//          ↓
//          ↓
// 2、一个链表有环，一个链表无环，因为是单向链表，这种情况不可能相交。
// 3、两个链表都有环
//    ① 有环但是不相交，记为 2b 结构。
//          ↓           ↓
//          ↓           ↓
//          ↓ ← ← ← ↑   ↓ ← ← ← ↑
//          ↓       ↑   ↓       ↑
//          ↓ → → → →   ↓ → → → →
//    ② 入环节点是同一个，记为 Y + b 结构。
//      ↘       ↙
//        ↘   ↙
//          ↓
//          ↓
//          ↓
//          ↓ ← ← ← ↑
//          ↓       ↑
//          ↓ → → → →
//    ③ 入环节点有两个，记为 b + d 结构。
//          ↓       ↓
//          ↓       ↓
//          ↓ ← ← ← ↑
//          ↓       ↑
//          ↓ → → → →

class FindFirstIntersectNode
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

    static Node* getIntersectNode(Node* head1, Node* head2)
    {
      if (head1 == NULL || head2 == NULL)
      {
        return NULL;
      }
      Node* loop1 = getLoopNode(head1);
      Node* loop2 = getLoopNode(head2);
      if (loop1 == NULL && loop2 == NULL)
      {
        return noLoop(head1, head2);
      }
      if (loop1 != NULL && loop2 != NULL)
      {
        return bothLoop(head1, loop1, head2, loop2);
      }
      return NULL;
    }

    // 找到链表第一个入环节点，如果无环，返回NULL
    static Node* getLoopNode(Node* head)
    {
      if (head == NULL || head->next == NULL || head->next->next == NULL)
      {
        return NULL;
      }
      // n1 慢  n2 快
      Node* slow = head->next; // n1 -> slow
      Node* fast = head->next->next; // n2 -> fast
      while (slow != fast)
      {
        if (fast->next == NULL || fast->next->next == NULL)
        {
          return NULL;
        }
        fast = fast->next->next;
        slow = slow->next;
      }
      // slow fast  相遇，这里并不是入环节点
      // 假设环外节点有m个，环内节点有n个，不妨设入环节点索引为0，
      // 慢指针走到入环节点（索引为0）时，需要走m步，快指针走2m步(索引为m % n，相当于在环内走了m步)，
      // 这个时候要相遇设慢指针在环内需要走y步，快指针就一共在环内走了2y+m步，
      // 可得y % n = (2y + m) % n，得y = - m % n，
      // 慢指针从0位置走到y位置才和快指针相遇，这时如果要回到0位置，只需要走回-y步即m % n 步。
      // 这个时候就让快指针从环外头结点开始，和慢指针每次同时走一步直到相遇就行了，相当于走了m步。
      fast = head; // n2 -> walk again from head
      while (slow != fast)
      {
        slow = slow->next;
        fast = fast->next;
      }
      return slow;
    }

    // 如果两个链表都无环，返回第一个相交节点，如果不想交，返回NULL
    static Node* noLoop(Node* head1, Node* head2)
    {
      if (head1 == NULL || head2 == NULL)
      {
        return NULL;
      }
      Node* cur1 = head1;
      Node* cur2 = head2;
      int n = 0;
      while (cur1->next != NULL)
      {
        n++;
        cur1 = cur1->next;
      }
      while (cur2->next != NULL)
      {
        n--;
        cur2 = cur2->next;
      }
      if (cur1 != cur2)  // Y 结构尾结点肯定相等
      {
        return NULL;
      }
      // n  :  链表1长度减去链表2长度的值
      cur1 = n > 0 ? head1 : head2; // 谁长，谁的头变成cur1
      cur2 = cur1 == head1 ? head2 : head1; // 谁短，谁的头变成cur2
      n = std::abs(n);
      while (n != 0)  // 长链表先走n步
      {
        n--;
        cur1 = cur1->next;
      }
      while (cur1 != cur2)  // 然后一起走，直到第一个相交的节点
      {
        cur1 = cur1->next;
        cur2 = cur2->next;
      }
      return cur1;
    }

    // 两个有环链表，返回第一个相交节点，如果不想交返回NULL
    static Node* bothLoop(Node* head1, Node* loop1, Node* head2, Node* loop2)
    {
      Node* cur1 = NULL;
      Node* cur2 = NULL;
      if (loop1 == loop2)  // Y + b 结构，直接忽略环，跟 Y 结构处理一样
      {
        cur1 = head1;
        cur2 = head2;
        int n = 0;
        while (cur1 != loop1)
        {
          n++;
          cur1 = cur1->next;
        }
        while (cur2 != loop2)
        {
          n--;
          cur2 = cur2->next;
        }
        cur1 = n > 0 ? head1 : head2;
        cur2 = cur1 == head1 ? head2 : head1;
        n = std::abs(n);
        while (n != 0)
        {
          n--;
          cur1 = cur1->next;
        }
        while (cur1 != cur2)
        {
          cur1 = cur1->next;
          cur2 = cur2->next;
        }
        return cur1;
      }
      else  //  2b 结构或者是b + d 结构
      {
        cur1 = loop1->next;
        while (cur1 != loop1)
        {
          if (cur1 == loop2) // 在回到loop1时遇到了loop2，说明是 b + d 结构
          {
            return loop1;  // 返回loop1或loop2都行
          }
          cur1 = cur1->next;
        }
        return NULL;  // 没遇到说明是 2b 结构
      }
    }

    static void test()
    {
      // 1->2->3->4->5->6->7->NULL
      Node* head1 = new Node(1);
      head1->next = new Node(2);
      head1->next->next = new Node(3);
      head1->next->next->next = new Node(4);
      head1->next->next->next->next = new Node(5);
      head1->next->next->next->next->next = new Node(6);
      head1->next->next->next->next->next->next = new Node(7);

      // 0->9->8->6->7->NULL
      Node* head2 = new Node(0);
      head2->next = new Node(9);
      head2->next->next = new Node(8);
      head2->next->next->next = head1->next->next->next->next->next; // 8->6
      cout << getIntersectNode(head1, head2)->value << endl;
      // 1->2->3->4->5->6->7->4...
      head1 = new Node(1);
      head1->next = new Node(2);
      head1->next->next = new Node(3);
      head1->next->next->next = new Node(4);
      head1->next->next->next->next = new Node(5);
      head1->next->next->next->next->next = new Node(6);
      head1->next->next->next->next->next->next = new Node(7);
      head1->next->next->next->next->next->next = head1->next->next->next; // 7->4

      // 0->9->8->2...
      head2 = new Node(0);
      head2->next = new Node(9);
      head2->next->next = new Node(8);
      head2->next->next->next = head1->next; // 8->2
      cout << getIntersectNode(head1, head2)->value << endl;

      // 0->9->8->6->4->5->6..
      head2 = new Node(0);
      head2->next = new Node(9);
      head2->next->next = new Node(8);
      head2->next->next->next = head1->next->next->next->next->next; // 8->6
      cout << getIntersectNode(head1, head2)->value << endl;
    }
};

int main()
{
  FindFirstIntersectNode::test();
  return 0;
}
