#include <stdbool.h>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <iostream>
#include <random>

using namespace std;

#define MAX_RANDOM 1024

int getRandom(int min, int max)
{
  random_device seed;  // 硬件生成随机数种子
  ranlux48 engine(seed());  // 利用种子生成随机数引
  uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
  int res = distrib(engine);  // 随机数
  return res;
}

// 跳表的节点定义
template <typename K, typename V>
class SkipListNode
{
  public:
    K key;
    V val;
    vector<SkipListNode<K, V>*> nextNodes;

    SkipListNode(K k, V v)
    {
      key = k;
      val = v;
    }

    // 遍历的时候，如果是往右遍历到的nullptr(next == nullptr), 遍历结束
    // 头(nullptr), 头节点的nullptr，认为最小
    // node  -> 头，node(nullptr, "")  node.isKeyLess(!nullptr)  true
    // node里面的key是否比otherKey小，true，不是false
    bool isKeyLess(K otherKey)
    {
      //  otherKey == nullptr -> false▫
      return otherKey != "" && (key == "" || key.compare(otherKey) < 0);
    }

    bool isKeyEqual(K otherKey)
    {
      return (key == "" && otherKey == "")                                                              || (key != "" && otherKey != "" && key.compare(otherKey) == 0);
    }

};

template <typename K, typename V>
class SkipListMap
{
  public:
    const double PROBABILITY = 0.5; // < 0.5 继续做，>=0.5 停
    SkipListNode<K, V>* head;
    int size;
    int maxLevel;

    SkipListMap()
    {
      head = new SkipListNode<K, V>("nullptr", "nullptr");
      head->nextNodes.push_back(nullptr); // 0
      size = 0;
      maxLevel = 0;
    }

    // 从最高层开始，一路找下去，
    // 最终，找到第0层的<key的最右的节点
    SkipListNode<K, V>* mostRightLessNodeInTree(K key)
    {
      if (key == "")
      {
        return nullptr;
      }
      int level = maxLevel;
      SkipListNode<K, V>* cur = head;
      while (level >= 0)
      {
        // 从上层跳下层
        //  cur  level  -> level-1
        cur = mostRightLessNodeInLevel(key, cur, level--);
      }
      return cur;
    }

    // 在level层里，如何往右移动
    // 现在来到的节点是cur，来到了cur的level层，在level层上，找到<key最后一个节点并返回
    SkipListNode<K, V>* mostRightLessNodeInLevel(K key,
        SkipListNode<K, V>* cur,
        int level)
    {
      SkipListNode<K, V>* next = cur->nextNodes.at(level);
      while (next != nullptr && next->isKeyLess(key))
      {
        cur = next;
        next = cur->nextNodes.at(level);
      }
      return cur;
    }

    bool containsKey(K key)
    {
      if (key == "")
      {
        return true;
      }
      SkipListNode<K, V>* less = mostRightLessNodeInTree(key);
      SkipListNode<K, V>* next = less->nextNodes.at(0);
      return next != nullptr && next->isKeyEqual(key);
    }

    // 新增、改value
    void put(K key, V value)
    {
      if (key == "")
      {
        return;
      }
      // 0层上，最右一个，< key 的Node -> >key
      SkipListNode<K, V>* less = mostRightLessNodeInTree(key);
      SkipListNode<K, V>* find = less->nextNodes.at(0);
      if (find != nullptr && find->isKeyEqual(key))
      {
        find->val = value;
      }
      else
      {
        // find == nullptr   8   7   9
        size++;
        int newNodeLevel = 0;
        while ((getRandom(0, MAX_RANDOM) / MAX_RANDOM) < PROBABILITY)
        {
          newNodeLevel++;
        }
        // newNodeLevel
        while (newNodeLevel > maxLevel)
        {
          head->nextNodes.push_back(nullptr);
          maxLevel++;
        }
        SkipListNode<K, V>* newNode = new SkipListNode<K, V>(key, value);
        for (int i = 0; i <= newNodeLevel; i++)
        {
          newNode->nextNodes.push_back(nullptr);
        }
        int level = maxLevel;
        SkipListNode<K, V>* pre = head;
        while (level >= 0)
        {
          // level 层中，找到最右的 < key 的节点
          pre = mostRightLessNodeInLevel(key, pre, level);
          if (level <= newNodeLevel)
          {
            newNode->nextNodes[level] = pre->nextNodes.at(level);
            pre->nextNodes[level] = newNode;
          }
          level--;
        }
      }
    }

    V get(K key)
    {
      if (key == "")
      {
        return "nullptr";
      }
      SkipListNode<K, V>* less = mostRightLessNodeInTree(key);
      SkipListNode<K, V>* next = less->nextNodes.at(0);
      return next != nullptr && next->isKeyEqual(key) ? next->val : "nullptr";
    }

    void remove(K key)
    {
      if (containsKey(key))
      {
        size--;
        int level = maxLevel;
        SkipListNode<K, V>* pre = head;
        while (level >= 0)
        {
          pre = mostRightLessNodeInLevel(key, pre, level);
          SkipListNode<K, V>* next = pre->nextNodes.at(level);
          // 1）在这一层中，pre下一个就是key
          // 2）在这一层中，pre的下一个key是>要删除key
          if (next != nullptr && next->isKeyEqual(key))
          {
            // free delete node memory -> C++
            // level : pre -> next(key) -> ...
            pre->nextNodes[level] =  next->nextNodes.at(level);
          }
          // 在level层只有一个节点了，就是默认节点head
          if (level != 0 && pre == head && pre->nextNodes.at(level) == nullptr)
          {
            head->nextNodes.erase(head->nextNodes.begin() + level - 1); // 删除 level
            maxLevel--;
          }
          level--;
        }
      }
    }
    K firstKey()
    {
      return head->nextNodes.at(0) != nullptr ? head->nextNodes.at(0)->key : "nullptr";
    }

    K lastKey()
    {
      int level = maxLevel;
      SkipListNode<K, V>* cur = head;
      while (level >= 0)
      {
        SkipListNode<K, V>* next = cur->nextNodes.at(level);
        while (next != nullptr)
        {
          cur = next;
          next = cur->nextNodes.at(level);
        }
        level--;
      }
      return cur->key;
    }

    K ceilingKey(K key)
    {
      if (key == "")
      {
        return "nullptr";
      }
      SkipListNode<K, V>* less = mostRightLessNodeInTree(key);
      SkipListNode<K, V>* next = less->nextNodes.at(0);
      return next != nullptr ? next->key : "nullptr";
    }

    K floorKey(K key)
    {
      if (key == "")
      {
        return "nullptr";
      }
      SkipListNode<K, V>* less = mostRightLessNodeInTree(key);
      SkipListNode<K, V>* next = less->nextNodes.at(0);
      return next != nullptr && next->isKeyEqual(key) ? next->key : less->key;
    }

    int sizes()
    {
      return size;
    }

};

// for test
void printAll(SkipListMap<string, string>* obj)
{
  for (int i = obj->maxLevel; i >= 0; i--)
  {
    cout << "Level " << i << " : ";
    SkipListNode<string, string>* cur = obj->head;
    while (cur->nextNodes.at(i) != nullptr)
    {
      SkipListNode<string, string>* next = cur->nextNodes.at(i);
      cout << "(" << next->key + " , " + next->val + ") ";
      cur = next;
    }
    cout << endl;
  }
}

int main()
{
  SkipListMap<string, string> test;
  printAll(&test);
  cout << "======================" << endl;
  test.put("A", "10");
  printAll(&test);
  cout << "======================" << endl;
  test.remove("A");
  printAll(&test);
  cout << "======================" << endl;
  test.put("E", "E");
  test.put("B", "B");
  test.put("A", "A");
  test.put("F", "F");
  test.put("C", "C");
  test.put("D", "D");
  printAll(&test);
  cout << "======================" << endl;
  cout << test.containsKey("B") << endl;
  cout << test.containsKey("Z") << endl;
  cout << test.firstKey() << endl;
  cout << test.lastKey() << endl;
  cout << test.floorKey("D") << endl;
  cout << test.ceilingKey("D") <<endl;
  cout << "======================" << endl;
  test.remove("D");
  //printAll(&test);
  cout << "======================" << endl;
  //cout << test.floorKey("D") << endl;
  cout << test.ceilingKey("D") << endl;
  return 0;
}
