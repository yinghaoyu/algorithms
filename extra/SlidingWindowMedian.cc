#include <stdbool.h>
#include <iostream>
#include <random>

using namespace std;

// 问题描述：给定一个数组arr，求滑动窗口的中位数
// 1 2 3 4 5 6 7 8
// {1 2 3} 4 5 6 7 8   {2}
// 1 {2 3 4} 5 6 7 8   {3}
// 1 2 {3 4 5} 6 7 8   {4}
// 1 2 3 {4 5 6} 7 8   {5}
// 1 2 3 4 {5 6 7} 8   {6}
// 1 2 3 4 5 {6 7 8}   {7}
//                     ans = {2, 3, 4, 5, 6, 7}
// 思想：通过改写Size Balanced Tree

class SlidingWindowMedian
{
 public:
  template <typename K>
  class SBTNode
  {
   public:
    K key;
    SBTNode<K> *l;
    SBTNode<K> *r;
    int size;

    SBTNode(K k)
    {
      key = k;
      size = 1;
      l = nullptr;
      r = nullptr;
    }
  };

  template <typename K>
  class SizeBalancedTreeMap
  {
   public:
    SBTNode<K> *root;

    SizeBalancedTreeMap() { root = nullptr; }

    SBTNode<K> *rightRotate(SBTNode<K> *cur)
    {
      SBTNode<K> *leftNode = cur->l;
      cur->l = leftNode->r;
      leftNode->r = cur;
      leftNode->size = cur->size;
      cur->size = (cur->l != nullptr ? cur->l->size : 0) + (cur->r != nullptr ? cur->r->size : 0) + 1;
      return leftNode;
    }

    SBTNode<K> *leftRotate(SBTNode<K> *cur)
    {
      SBTNode<K> *rightNode = cur->r;
      cur->r = rightNode->l;
      rightNode->l = cur;
      rightNode->size = cur->size;
      cur->size = (cur->l != nullptr ? cur->l->size : 0) + (cur->r != nullptr ? cur->r->size : 0) + 1;
      return rightNode;
    }

    SBTNode<K> *maintain(SBTNode<K> *cur)
    {
      if (cur == nullptr)
      {
        return nullptr;
      }
      int leftSize = cur->l != nullptr ? cur->l->size : 0;
      int leftLeftSize = cur->l != nullptr && cur->l->l != nullptr ? cur->l->l->size : 0;
      int leftRightSize = cur->l != nullptr && cur->l->r != nullptr ? cur->l->r->size : 0;
      int rightSize = cur->r != nullptr ? cur->r->size : 0;
      int rightLeftSize = cur->r != nullptr && cur->r->l != nullptr ? cur->r->l->size : 0;
      int rightRightSize = cur->r != nullptr && cur->r->r != nullptr ? cur->r->r->size : 0;
      if (leftLeftSize > rightSize)
      {
        cur = rightRotate(cur);
        cur->r = maintain(cur->r);
        cur = maintain(cur);
      }
      else if (leftRightSize > rightSize)
      {
        cur->l = leftRotate(cur->l);
        cur = rightRotate(cur);
        cur->l = maintain(cur->l);
        cur->r = maintain(cur->r);
        cur = maintain(cur);
      }
      else if (rightRightSize > leftSize)
      {
        cur = leftRotate(cur);
        cur->l = maintain(cur->l);
        cur = maintain(cur);
      }
      else if (rightLeftSize > leftSize)
      {
        cur->r = rightRotate(cur->r);
        cur = leftRotate(cur);
        cur->l = maintain(cur->l);
        cur->r = maintain(cur->r);
        cur = maintain(cur);
      }
      return cur;
    }

    SBTNode<K> *findLastIndex(K key)
    {
      SBTNode<K> *pre = root;
      SBTNode<K> *cur = root;
      while (cur != nullptr)
      {
        pre = cur;
        if (key == cur->key)
        {
          break;
        }
        else if (key < cur->key)
        {
          cur = cur->l;
        }
        else
        {
          cur = cur->r;
        }
      }
      return pre;
    }

    SBTNode<K> *add(SBTNode<K> *cur, K key)
    {
      if (cur == nullptr)
      {
        return new SBTNode<K>(key);
      }
      else
      {
        cur->size++;
        if (key < cur->key)
        {
          cur->l = add(cur->l, key);
        }
        else
        {
          cur->r = add(cur->r, key);
        }
        return maintain(cur);
      }
    }

    SBTNode<K> *del(SBTNode<K> *cur, K key)
    {
      cur->size--;
      if (key > cur->key)
      {
        cur->r = del(cur->r, key);
      }
      else if (key < cur->key)
      {
        cur->l = del(cur->l, key);
      }
      else
      {
        if (cur->l == nullptr && cur->r == nullptr)
        {
          // free cur memory -> C++
          cur = nullptr;
        }
        else if (cur->l == nullptr && cur->r != nullptr)
        {
          // free cur memory -> C++
          cur = cur->r;
        }
        else if (cur->l != nullptr && cur->r == nullptr)
        {
          // free cur memory -> C++
          cur = cur->l;
        }
        else
        {
          SBTNode<K> *pre = nullptr;
          SBTNode<K> *des = cur->r;
          des->size--;
          while (des->l != nullptr)
          {
            pre = des;
            des = des->l;
            des->size--;
          }
          if (pre != nullptr)
          {
            pre->l = des->r;
            des->r = cur->r;
          }
          des->l = cur->l;
          des->size = des->l->size + (des->r == nullptr ? 0 : des->r->size) + 1;
          // free cur memory -> C++
          cur = des;
        }
      }
      return cur;
    }

    SBTNode<K> *getIndex(SBTNode<K> *cur, int kth)
    {
      if (kth == (cur->l != nullptr ? cur->l->size : 0) + 1)
      {
        return cur;
      }
      else if (kth <= (cur->l != nullptr ? cur->l->size : 0))
      {
        return getIndex(cur->l, kth);
      }
      else
      {
        return getIndex(cur->r, kth - (cur->l != nullptr ? cur->l->size : 0) - 1);
      }
    }

    int size() { return root == nullptr ? 0 : root->size; }

    bool containsKey(K key)
    {
      SBTNode<K> *lastNode = findLastIndex(key);
      return lastNode != nullptr && key < lastNode->key == 0 ? true : false;
    }

    void add(K key)
    {
      SBTNode<K> *lastNode = findLastIndex(key);
      if (lastNode == nullptr || key != lastNode->key)
      {
        root = add(root, key);
      }
    }

    void remove(K key)
    {
      if (containsKey(key))
      {
        root = del(root, key);
      }
    }

    K getIndexKey(int index)
    {
      if (index < 0 || index >= this->size())
      {
        throw "invalid parameter.";
      }
      return getIndex(root, index + 1)->key;
    }
  };

  class Node
  {
   public:
    int index;
    int value;

    Node()
    {
      index = 0;
      value = 0;
    }

    Node(int i, int v)
    {
      index = i;
      value = v;
    }

    bool operator<(const Node &other) { return value != other.value ? value < other.value : index < other.index; }

    bool operator>(const Node &other) { return value != other.value ? value > other.value : index > other.index; }

    bool operator==(const Node &other) { return value == other.value && index == other.index; }

    bool operator!=(const Node &other) { return !(*this == other); }
  };

  static double *medianSlidingWindow(int *nums, int len, int k)
  {
    SizeBalancedTreeMap<Node> map;
    for (int i = 0; i < k - 1; i++)
    {
      map.add(Node(i, nums[i]));
    }
    double *ans = new double[len - k + 1]();
    int index = 0;
    for (int i = k - 1; i < len; i++)
    {
      map.add(Node(i, nums[i]));
      if (map.size() % 2 == 0)
      {
        Node upmid = map.getIndexKey(map.size() / 2 - 1);
        Node downmid = map.getIndexKey(map.size() / 2);
        ans[index++] = ((double) upmid.value + (double) downmid.value) / 2;
      }
      else
      {
        Node mid = map.getIndexKey(map.size() / 2);
        ans[index++] = (double) mid.value;
      }
      map.remove(Node(i - k + 1, nums[i - k + 1]));
    }
    return ans;
  }
};

int main()
{
  int arr[] = {1, 2, 4, 3, 5, 6, 7, 8, 9};
  int len = sizeof(arr) / sizeof(int);
  int wsize = 3;
  double *res = SlidingWindowMedian::medianSlidingWindow(arr, len, wsize);
  for (int i = 0; i < len - wsize + 1; i++)
  {
    cout << res[i] << " ";
  }
  cout << endl;
  return 0;
}
