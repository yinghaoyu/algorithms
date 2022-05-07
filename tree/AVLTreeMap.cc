#include <iostream>
#include <unordered_map>

using namespace std;

template <typename K, typename V>
class AVLNode
{
  public:
    K k;
    V v;
    AVLNode<K, V>* l;
    AVLNode<K, V>* r;
    int h;

    AVLNode(K key, V value)
    {
      k = key;
      v = value;
      h = 1;
    }
};

template <typename K, typename V>
class AVLTreeMap
{
  private:
    AVLNode<K, V>* root;
    int count;

  public:
    AVLTreeMap()
    {
      root = nullptr;
      count = 0;
    }

    AVLNode<K, V>* rightRotate(AVLNode<K, V>* cur)
    {
      AVLNode<K, V>* left = cur->l;
      cur->l = left->r;
      left->r = cur;
      cur->h = std::max((cur->l != nullptr ? cur->l->h : 0), (cur->r != nullptr ? cur->r->h : 0)) + 1;
      left->h = std::max((left->l != nullptr ? left->l->h : 0), (left->r != nullptr ? left->r->h : 0)) + 1;
      return left;
    }

    AVLNode<K, V>* leftRotate(AVLNode<K, V>* cur)
    {
      AVLNode<K, V>* right = cur->r;
      cur->r = right->l;
      right->l = cur;
      cur->h = std::max((cur->l != nullptr ? cur->l->h : 0), (cur->r != nullptr ? cur->r->h : 0)) + 1;
      right->h = std::max((right->l != nullptr ? right->l->h : 0), (right->r != nullptr ? right->r->h : 0)) + 1;
      return right;
    }

    AVLNode<K, V>* maintain(AVLNode<K, V>* cur)
    {
      if (cur == nullptr) 
      {
        return nullptr;
      }
      int leftHeight = cur->l != nullptr ? cur->l->h : 0;
      int rightHeight = cur->r != nullptr ? cur->r->h : 0;
      if (std::abs(leftHeight - rightHeight) > 1)
      {
        if (leftHeight > rightHeight)
        {
          int leftLeftHeight = cur->l != nullptr && cur->l->l != nullptr ? cur->l->l->h : 0;
          int leftRightHeight = cur->l != nullptr && cur->l->r != nullptr ? cur->l->r->h : 0;
          if (leftLeftHeight >= leftRightHeight)
          {
            cur = rightRotate(cur);
          }
          else
          {
            cur->l = leftRotate(cur->l);
            cur = rightRotate(cur);
          }
        }
        else
        {
          int rightLeftHeight = cur->r != nullptr && cur->r->l != nullptr ? cur->r->l->h : 0;
          int rightRightHeight = cur->r != nullptr && cur->r->r != nullptr ? cur->r->r->h : 0;
          if (rightRightHeight >= rightLeftHeight)
          {
            cur = leftRotate(cur);
          }
          else
          {
            cur->r = rightRotate(cur->r);
            cur = leftRotate(cur);
          }
        }
      }
      return cur;
    }

    AVLNode<K, V>* findLastIndex(K key)
    {
      AVLNode<K, V>* pre = root;
      AVLNode<K, V>* cur = root;
      while (cur != nullptr)
      {
        pre = cur;
        if (key.compareTo(cur->k) == 0)
        {
          break;
        }
        else if (key.compareTo(cur->k) < 0)
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

    AVLNode<K, V>* findLastNoSmallIndex(K key)
    {
      AVLNode<K, V>* ans = nullptr;
      AVLNode<K, V>* cur = root;
      while (cur != nullptr)
      {
        if (key.compareTo(cur->k) == 0)
        {
          ans = cur;
          break;
        }
        else if (key.compareTo(cur->k) < 0)
        {
          ans = cur;
          cur = cur->l;
        }
        else
        {
          cur = cur->r;
        }
      }
      return ans;
    }

    AVLNode<K, V>* findLastNoBigIndex(K key)
    {
      AVLNode<K, V>* ans = nullptr;
      AVLNode<K, V>* cur = root;
      while (cur != nullptr)
      {
        if (key.compareTo(cur->k) == 0)
        {
          ans = cur;
          break;
        }
        else if (key.compareTo(cur->k) < 0)
        {
          cur = cur->l;
        }
        else
        {
          ans = cur;
          cur = cur->r;
        }
      }
      return ans;
    }

    AVLNode<K, V>* add(AVLNode<K, V>* cur, K key, V value)
    {
      if (cur == nullptr)
      {
        return new AVLNode<K, V>(key, value);
      }
      else
      {
        if (key.compareTo(cur->k) < 0)
        {
          cur->l = add(cur->l, key, value);
        }
        else
        {
          cur->r = add(cur->r, key, value);
        }
        cur->h = std::max(cur->l != nullptr ? cur->l->h : 0, cur->r != nullptr ? cur->r->h : 0) + 1;
        return maintain(cur);
      }
    }
    // 在cur这棵树上，删掉key所代表的节点
    // 返回cur这棵树的新头部
    AVLNode<K, V>* del(AVLNode<K, V>* cur, K key)
    {
      if (key.compareTo(cur->k) > 0)
      {
        cur->r = del(cur->r, key);
      }
      else if (key.compareTo(cur->k) < 0)
      {
        cur->l = del(cur->l, key);
      }
      else
      {
        if (cur->l == nullptr && cur->r == nullptr)
        {
          cur = nullptr;
        }
        else if (cur->l == nullptr && cur->r != nullptr)
        {
          cur = cur->r;
        } else if (cur->l != nullptr && cur->r == nullptr)
        {
          cur = cur->l;
        }
        else
        {
          AVLNode<K, V>* des = cur->r;
          while (des->l != nullptr)
          {
            des = des->l;
          }
          cur->r = del(cur->r, des->k);
          des->l = cur->l;
          des->r = cur->r;
          cur = des;
        }
      }
      if (cur != nullptr)
      {
        cur->h = std::max(cur->l != nullptr ? cur->l->h : 0, cur->r != nullptr ? cur->r->h : 0) + 1;
      }
      return maintain(cur);
    }

    int size()
    {
      return count;
    }

    bool containsKey(K key)
    {
      if (key == nullptr)
      {
        return false;
      }
      AVLNode<K, V>* lastNode = findLastIndex(key);
      return lastNode != nullptr && key.compareTo(lastNode->k) == 0 ? true : false;
    }

    void put(K key, V value)
    {
      if (key == nullptr)
      {
        return;
      }
      AVLNode<K, V>* lastNode = findLastIndex(key);
      if (lastNode != nullptr && key.compareTo(lastNode->k) == 0)
      {
        lastNode->v = value;
      }
      else
      {
        count++;
        root = add(root, key, value);
      }
    }
    void remove(K key)
    {
      if (key == nullptr)
      {
        return;
      }
      if (containsKey(key))
      {
        count--;
        root = del(root, key);
      }
    }

    V get(K key)
    {
      if (key == nullptr)
      {
        return nullptr;
      }
      AVLNode<K, V>* lastNode = findLastIndex(key);
      if (lastNode != nullptr && key.compareTo(lastNode->k) == 0)
      {
        return lastNode->v;
      }
      return nullptr;
    }
    K firstKey()
    {
      if (root == nullptr)
      {
        return nullptr;
      }
      AVLNode<K, V>* cur = root;
      while (cur->l != nullptr)
      {
        cur = cur->l;
      }
      return cur->k;
    }

    K lastKey()
    {
      if (root == nullptr) {
        return nullptr;
      }
      AVLNode<K, V>* cur = root;
      while (cur->r != nullptr)
      {
        cur = cur->r;
      }
      return cur->k;
    }
    K floorKey(K key)
    {
      if (key == nullptr) {
        return nullptr;
      }
      AVLNode<K, V>* lastNoBigNode = findLastNoBigIndex(key);
      return lastNoBigNode == nullptr ? nullptr : lastNoBigNode->k;
    }

    K ceilingKey(K key)
    {
      if (key == nullptr)
      {
        return nullptr;
      }
      AVLNode<K, V> lastNoSmallNode = findLastNoSmallIndex(key);
      return lastNoSmallNode == nullptr ? nullptr : lastNoSmallNode.k;
    }
};
