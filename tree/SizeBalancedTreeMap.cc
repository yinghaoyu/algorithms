#include <iostream>
#include <limits.h>
#include <queue>
#include <vector>
#include <unordered_map>
#include <unordered_set>

using namespace std;

// 每棵树的子树的size不小于其子树兄弟的子树size，即满足
// cur->left->size >= cur->right->left->size
// cur->left->size >= cur->right->right->size
// 同时
// cur->right->size >= cur->left->left->size
// cur->right->size >= cur->left->right->size
template <typename K, typename V>
class SBTNode
{
  public:
    K key;
    V value;
    SBTNode<K, V>* l;
    SBTNode<K, V>* r;
    int size; // 不同的key的数量

    SBTNode(K key, V value)
    {
      this->key = key;
      this->value = value;
      size = 1;
    }
};

template<typename K, typename V>
class SizeBalancedTreeMap
{
public:
    SBTNode<K, V>* root;

    SizeBalancedTreeMap()
    {
      root = NULL;
    }

    SBTNode<K, V>* rightRotate(SBTNode<K, V>* cur)
    {
      SBTNode<K, V>* leftNode = cur->l;
      cur->l = leftNode->r;
      leftNode->r = cur;
      leftNode->size = cur->size;
      cur->size = (cur->l != NULL ? cur->l->size : 0) + (cur->r != NULL ? cur->r->size : 0) + 1;
      return leftNode;
    }

    SBTNode<K, V>* leftRotate(SBTNode<K, V>* cur)
    {
      SBTNode<K, V>* rightNode = cur->r;
      cur->r = rightNode->l;
      rightNode->l = cur;
      rightNode->size = cur->size;
      cur->size = (cur->l != NULL ? cur->l->size : 0) + (cur->r != NULL ? cur->r->size : 0) + 1;
      return rightNode;
    }
    SBTNode<K, V>* maintain(SBTNode<K, V>* cur)
    {
      if (cur == NULL)
      {
        return NULL;
      }
      int leftSize = cur->l != NULL ? cur->l->size : 0;
      int leftLeftSize = cur->l != NULL && cur->l->l != NULL ? cur->l->l->size : 0;
      int leftRightSize = cur->l != NULL && cur->l->r != NULL ? cur->l->r->size : 0;
      int rightSize = cur->r != NULL ? cur->r->size : 0;
      int rightLeftSize = cur->r != NULL && cur->r->l != NULL ? cur->r->l->size : 0;
      int rightRightSize = cur->r != NULL && cur->r->r != NULL ? cur->r->r->size : 0;
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

    SBTNode<K, V>* findLastIndex(K key)
    {
      SBTNode<K, V>* pre = root;
      SBTNode<K, V>* cur = root;
      while (cur != NULL) {
        pre = cur;
        if (key.compare(cur->key) == 0)
        {
          break;
        }
        else if (key.compare(cur->key) < 0)
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

    SBTNode<K, V>* findLastNoSmallIndex(K key)
    {
      SBTNode<K, V>* ans = NULL;
      SBTNode<K, V>* cur = root;
      while (cur != NULL) {
        if (key.compare(cur->key) == 0)
        {
          ans = cur;
          break;
        }
        else if (key.compare(cur->key) < 0)
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

    SBTNode<K, V>* findLastNoBigIndex(K key)
    {
      SBTNode<K, V>* ans = NULL;
      SBTNode<K, V>* cur = root;
      while (cur != NULL)
      {
        if (key.compare(cur->key) == 0)
        {
          ans = cur;
          break;
        }
        else if (key.compare(cur->key) < 0)
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

    // 现在，以cur为头的树上，新增，加(key, value)这样的记录
    // 加完之后，会对cur做检查，该调整调整
    // 返回，调整完之后，整棵树的新头部
    SBTNode<K, V>* add(SBTNode<K, V>* cur, K key, V value)
    {
      if (cur == NULL)
      {
        return new SBTNode<K, V>(key, value);
      }
      else
      {
        cur->size++;
        if (key.compare(cur->key) < 0)
        {
          cur->l = add(cur->l, key, value);
        }
        else
        {
          cur->r = add(cur->r, key, value);
        }
        return maintain(cur);
      }
    }

    // 在cur这棵树上，删掉key所代表的节点
    // 返回cur这棵树的新头部
    SBTNode<K, V>* del(SBTNode<K, V>* cur, K key)
    {
      cur->size--;
      if (key.compare(cur->key) > 0)
      {
        cur->r = del(cur->r, key);
      }
      else if (key.compare(cur->key) < 0)
      {
        cur->l = del(cur->l, key);
      }
      else
      {
        // 当前要删掉cur
        if (cur->l == NULL && cur->r == NULL)
        {
          // free cur memory -> C++
          cur = NULL;
        }
        else if (cur->l == NULL && cur->r != NULL)
        {
          // free cur memory -> C++
          cur = cur->r;
        }
        else if (cur->l != NULL && cur->r == NULL)
        {
          // free cur memory -> C++
          cur = cur->l;
        }
        else
        {
          // 有左有右
          SBTNode<K, V>* pre = NULL;
          SBTNode<K, V>* des = cur->r;
          des->size--;
          while (des->l != NULL)
          {
            pre = des;
            des = des->l;
            des->size--;
          }
          if (pre != NULL)
          {
            pre->l = des->r;
            des->r = cur->r;
          }
          des->l = cur->l;
          des->size = des->l->size + (des->r == NULL ? 0 : des->r->size) + 1;
          // free cur memory -> C++
          cur = des;
        }
      }
      // cur = maintain(cur);
      return cur;
    }

    SBTNode<K, V>* getIndex(SBTNode<K, V>* cur, int kth)
    {
      if (kth == (cur->l != NULL ? cur->l->size : 0) + 1) 
      {
        return cur;
      }
      else if (kth <= (cur->l != NULL ? cur->l->size : 0))
      {
        return getIndex(cur->l, kth);
      }
      else
      {
        return getIndex(cur->r, kth - (cur->l != NULL ? cur->l->size : 0) - 1);
      }
    }

    int size()
    {
      return root == NULL ? 0 : root->size;
    }

    bool containsKey(K key)
    {
      if (key == "")
      {
        return true;
      }
      SBTNode<K, V>* lastNode = findLastIndex(key);
      return lastNode != NULL && key.compare(lastNode->key) == 0 ? true : false;
    }

    // （key，value） put -> 有序表 新增、改value
    void put(K key, V value)
    {
      if (key == "")
      {
        return;
      }
      SBTNode<K, V>* lastNode = findLastIndex(key);
      if (lastNode != NULL && key.compare(lastNode->key) == 0)
      {
        lastNode->value = value;
      }
      else
      {
        root = add(root, key, value);
      }
    }

    void remove(K key)
    {
      if (key == "")
      {
        return;
      }
      if (containsKey(key))
      {
        root = del(root, key);
      }
    }

    K getIndexKey(int index)
    {
      if (index < 0 || index >= this->size())
      {
        return "NULL";
      }
      return getIndex(root, index + 1)->key;
    }

    V getIndexValue(int index)
    {
      if (index < 0 || index >= this->size())
      {
        return INT_MAX;
      }
      return getIndex(root, index + 1)->value;
    }

    V get(K key)
    {
      if (key == "")
      {
        return INT_MAX;
      }
      SBTNode<K, V>* lastNode = findLastIndex(key);
      if (lastNode != NULL && key.compare(lastNode->key) == 0)
      {
        return lastNode->value;
      }
      else
      {
        return INT_MAX;
      }
    }

    K firstKey()
    {
      if (root == NULL)
      {
        return "NULL";
      }
      SBTNode<K, V>* cur = root;
      while (cur->l != NULL)
      {
        cur = cur->l;
      }
      return cur->key;
    }

    K lastKey()
    {
      if (root == NULL)
      {
        return "NULL";
      }
      SBTNode<K, V>* cur = root;
      while (cur->r != NULL)
      {
        cur = cur->r;
      }
      return cur->key;
    }

    K floorKey(K key)
    {
      if (key == "")
      {
        return "NULL";
      }
      SBTNode<K, V>* lastNoBigNode = findLastNoBigIndex(key);
      return lastNoBigNode == NULL ? "NULL" : lastNoBigNode->key;
    }

    K ceilingKey(K key)
    {
      if (key == "")
      {
        return "NULL";
      }
      SBTNode<K, V>* lastNoSmallNode = findLastNoSmallIndex(key);
      return lastNoSmallNode == NULL ? "NULL" : lastNoSmallNode->key;
    }

    // for test
    void printAll(SBTNode<string, int>* head) 
    {
      cout<< "Binary Tree:" << endl;
      printInOrder(head, 0, "H", 17);
      cout << endl;
    }

    // for test
    void printInOrder(SBTNode<string, int>* head, int height, string to, int len)
    {
      if (head == NULL)
      {
        return;
      }
      printInOrder(head->r, height + 1, "v", len);
      string val = to + "(" + head->key + "," + std::to_string(head->value) + ")" + to;
      int lenM = val.length();
      int lenL = (len - lenM) / 2;
      int lenR = len - lenM - lenL;
      val = getSpace(lenL) + val + getSpace(lenR);
      cout << getSpace(height * len) + val << endl;
      printInOrder(head->l, height + 1, "^", len);
    }
    // for test
    string getSpace(int num)
    {
      return string(num, ' ');
    }
};

int main()
{
  SizeBalancedTreeMap<string, int> sbt;
  sbt.put("d", 4);
  sbt.put("c", 3);
  sbt.put("a", 1);
  sbt.put("b", 2);
  // sbt.put("e", 5);
  sbt.put("g", 7);
  sbt.put("f", 6);
  sbt.put("h", 8);
  sbt.put("i", 9);
  sbt.put("a", 111);
  cout << sbt.get("a") << endl;
  sbt.put("a", 1);
  cout << sbt.get("a") << endl;
  for (int i = 0; i < sbt.size(); i++)
  {
    cout << sbt.getIndexKey(i) << " , " << sbt.getIndexValue(i) << endl;
  }
  sbt.printAll(sbt.root);
  cout << sbt.firstKey() << endl;
  cout<<sbt.lastKey() << endl;
  cout<<sbt.floorKey("g") << endl;
  cout<<sbt.ceilingKey("g") << endl;
  cout<<sbt.floorKey("e") << endl;
  cout<<sbt.ceilingKey("e") << endl;
  cout<<sbt.floorKey("") << endl;
  cout<<sbt.ceilingKey("") << endl;
  cout<<sbt.floorKey("j") << endl;
  cout<<sbt.ceilingKey("j") << endl;
  sbt.remove("d");
  sbt.printAll(sbt.root);
  sbt.remove("f");
  sbt.printAll(sbt.root);
  return 0;
}
