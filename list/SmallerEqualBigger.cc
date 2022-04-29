#include <iostream>
#include <stdbool.h>

using namespace std;

// 将单向链表划分成左边小、中间等、右边大的形式
//
// 1、可以采用荷兰国旗问题来求解，只是此时的额外空间复杂度为O(N)
//
// 2、额外空间复杂度为O(1)的解法：
//（1）使用6个变量，依次表示小于区头节点、尾结点，等于区头节点、尾结点，大于区头节点、尾结点
//（2）将链表分成 小于区 等于区 大于区 三块
//（3）将小于区 等于区 大于区 三块头尾连接起来

class SmallerEqualBigger
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

    static Node* listPartition1(Node* head, int pivot)
    {
      if (head == NULL)
      {
        return head;
      }
      Node* cur = head;
      int i = 0;
      while (cur != NULL)
      {
        i++;
        cur = cur->next;
      }
      int len = i;
      Node** nodeArr = (Node**)malloc(i * sizeof(Node*));
      i = 0;
      cur = head;
      for (i = 0; i != len; i++)
      {
        nodeArr[i] = cur;
        cur = cur->next;
      }
      arrPartition(nodeArr, len, pivot);
      for (i = 1; i != len; i++)
      {
        nodeArr[i - 1]->next = nodeArr[i];
      }
      nodeArr[i - 1]->next = NULL;
      return nodeArr[0];
    }

    static void arrPartition(Node** nodeArr, int len, int pivot)
    {
      int small = -1;
      int big = len;
      int index = 0;
      while (index != big)
      {
        if (nodeArr[index]->value < pivot)
        {
          swap(nodeArr, ++small, index++);
        }
        else if (nodeArr[index]->value == pivot)
        {
          index++;
        }
        else
        {
          swap(nodeArr, --big, index);
        }
      }
    }

    static void swap(Node** nodeArr, int a, int b)
    {
      Node* tmp = nodeArr[a];
      nodeArr[a] = nodeArr[b];
      nodeArr[b] = tmp;
    }

    static Node* listPartition2(Node* head, int pivot) {
      Node* sH = NULL; // small head
      Node* sT = NULL; // small tail
      Node* eH = NULL; // equal head
      Node* eT = NULL; // equal tail
      Node* mH = NULL; // big head
      Node* mT = NULL; // big tail
      Node* next = NULL; // save->next node
                         // every node distributed to three lists
      while (head != NULL)
      {
        next = head->next;
        head->next = NULL;
        if (head->value < pivot)
        {
          if (sH == NULL)
          {
            sH = head;
            sT = head;
          }
          else
          {
            sT->next = head;
            sT = head;
          }
        }
        else if (head->value == pivot)
        {
          if (eH == NULL)
          {
            eH = head;
            eT = head;
          }
          else
          {
            eT->next = head;
            eT = head;
          }
        }
        else
        {
          if (mH == NULL)
          {
            mH = head;
            mT = head;
          }
          else
          {
            mT->next = head;
            mT = head;
          }
        }
        head = next;
      }
      // 小于区域的尾巴，连等于区域的头，等于区域的尾巴连大于区域的头
      if (sT != NULL)
      {
        // 如果有小于区域
        sT->next = eH;
        eT = eT == NULL ? sT : eT; // 下一步，谁去连大于区域的头，谁就变成eT
      }
      // 下一步，一定是需要用eT 去接 大于区域的头
      // 有等于区域，eT -> 等于区域的尾结点
      // 无等于区域，eT -> 小于区域的尾结点
      // eT 尽量不为空的尾巴节点
      if (eT != NULL)
      {
        // 如果小于区域和等于区域，不是都没有
        eT->next = mH;
      }
      return sH != NULL ? sH : (eH != NULL ? eH : mH);
    }

    static void printLinkedList(Node* node)
    {
      cout << "Linked List: ";
      while (node != NULL)
      {
        cout << node->value << " ";
        node = node->next;
      }
      cout << endl;
    }
};


int main()
{
  SmallerEqualBigger::Node* head1 = new SmallerEqualBigger::Node(7);
  head1->next = new SmallerEqualBigger::Node(9);
  head1->next->next = new SmallerEqualBigger::Node(1);
  head1->next->next->next = new SmallerEqualBigger::Node(8);
  head1->next->next->next->next = new SmallerEqualBigger::Node(5);
  head1->next->next->next->next->next = new SmallerEqualBigger::Node(2);
  head1->next->next->next->next->next->next = new SmallerEqualBigger::Node(5);
  SmallerEqualBigger::printLinkedList(head1);
  //head1 = SmallerEqualBigger::listPartition1(head1, 4);
  head1 = SmallerEqualBigger::listPartition2(head1, 4);
  SmallerEqualBigger::printLinkedList(head1);
  return 0;
}
