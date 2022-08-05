#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

// 问题描述：
// 请你设计并实现一个满足  LRU (最近最少使用) 缓存 约束的数据结构。
// 实现 LRUCache 类：
// LRUCache(int capacity) 以 正整数 作为容量 capacity 初始化 LRU 缓存
// int get(int key) 如果关键字 key 存在于缓存中，则返回关键字的值，否则返回 -1 。
// void put(int key, int value) 如果关键字 key 已经存在，则变更其数据值 value ；如果不存在，则向缓存中插入该组 key-value。
// 如果插入操作导致关键字数量超过 capacity ，则应该 逐出 最久未使用的关键字。 函数 get 和 put 必须以 O(1) 的平均时间复杂度运行。

// 思路：
// HashMap + 双向链表
// 1、get的时候，利用HashMap直接找那个节点，并把该节点调整到双向链表头部
// 2、put的时候，先在HashMap找，找到就更新，并把节点调整到双向链表尾部
//    未找到就增加到HashMap中，并连接到双向链表尾部，超过capacity，就把双向链表头部往后移动

// 测试链接 : https://leetcode.com/problems/lru-cache/

class LRUCache
{
 public:
  LRUCache(int capacity) { cache = MyCache<int, int>(capacity); }

 public:
  int get(int key) { return cache.get(key); }

  void put(int key, int value) { cache.set(key, value); }

  template <typename K, typename V>
  class Node
  {
   public:
    K key;

    V value;

    Node<K, V> *last;

    Node<K, V> *next;

    Node(K key, V value)
    {
      this->key = key;
      this->value = value;
    }
  };

  template <typename K, typename V>
  class NodeDoubleLinkedList
  {
   private:
    Node<K, V> *head;

    Node<K, V> *tail;

   public:
    NodeDoubleLinkedList()
    {
      head = nullptr;
      tail = nullptr;
    }

   public:
    // 现在来了一个新的node，请挂到尾巴上去
    void addNode(Node<K, V> *newNode)
    {
      if (newNode == nullptr)
      {
        return;
      }
      if (head == nullptr)
      {
        head = newNode;
        tail = newNode;
      }
      else
      {
        tail->next = newNode;
        newNode->last = tail;
        tail = newNode;
      }
    }

    // node 入参，一定保证！node在双向链表里！
    // node原始的位置，左右重新连好，然后把node分离出来
    // 挂到整个链表的尾巴上
    void moveNodeToTail(Node<K, V> *node)
    {
      if (tail == node)
      {
        return;
      }
      if (head == node)
      {
        head = node->next;
        head->last = nullptr;
      }
      else
      {
        node->last->next = node->next;
        node->next->last = node->last;
      }
      node->last = tail;
      node->next = nullptr;
      tail->next = node;
      tail = node;
    }

    Node<K, V> *removeHead()
    {
      if (head == nullptr)
      {
        return nullptr;
      }
      Node<K, V> *res = head;
      if (head == tail)
      {
        head = nullptr;
        tail = nullptr;
      }
      else
      {
        head = res->next;
        res->next = nullptr;
        head->last = nullptr;
      }
      return res;
    }
  };

  template <typename K, typename V>
  class MyCache
  {
   private:
    unordered_map<K, Node<K, V> *> keyNodeMap;
    NodeDoubleLinkedList<K, V> nodeList;
    int capacity;

   public:
    MyCache() { capacity = 0; }
    MyCache(int cap) { capacity = cap; }

    V get(K key)
    {
      if (keyNodeMap.find(key) != keyNodeMap.end())
      {
        Node<K, V> *res = keyNodeMap.at(key);
        nodeList.moveNodeToTail(res);
        return res->value;
      }
      return -1;
    }

    // set(Key, Value)
    // 新增  更新value的操作
    void set(K key, V value)
    {
      if (keyNodeMap.find(key) != keyNodeMap.end())
      {
        Node<K, V> *node = keyNodeMap.at(key);
        node->value = value;
        nodeList.moveNodeToTail(node);
      }
      else
      {  // 新增！
        Node<K, V> *newNode = new Node<K, V>(key, value);
        keyNodeMap[key] = newNode;
        nodeList.addNode(newNode);
        if (keyNodeMap.size() == capacity + 1)
        {
          removeMostUnusedCache();
        }
      }
    }

    void removeMostUnusedCache()
    {
      Node<K, V> *removeNode = nodeList.removeHead();
      keyNodeMap.erase(removeNode->key);
    }
  };

 private:
  MyCache<int, int> cache;
};
