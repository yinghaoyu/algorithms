#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// KMP是用于一对一的字符串匹配
// trie虽然能用于多模式匹配，但是每次匹配失败都需要进行回溯，如果模式串很长的话会很浪费时间
// 所以AC自动机应运而生,如同Manacher一样，AC自动机利用某些操作阻止了模式串匹配阶段的回溯，将时间复杂度优化到了O(N) N为文本串长度

// AC自动机是在字典树的基础上，加上了fail指针
//           root
//        ↙    ↓    ↘       bfs     a->fail = root      bfs
//       a     s      b      -->    s->fail = root   -->  s'->fail = s
//     ↙       ↓       ↘            b->fall = root
//    s'       h         c
//
// 根节点首字母的fail指向根节点，第一个字母都匹配失败了
// s'的fail指向父节点a的fail(即root)与s'相同的子节点(即s)

// https://blog.csdn.net/bestsort/article/details/82947639

// 应用场景：热点推送
// 水果关键词：香蕉、苹果、樱桃
// 问卷：我喜欢的水果是苹果和樱桃
// 根据问卷，定位关键词

class AC1
{
 public:
  class Node
  {
   public:
    int end;     // 有多少个字符串以该节点结尾
    Node *fail;  // 失配指针
    unordered_map<int, Node *> nexts;

    Node()
    {
      end = 0;
      fail = nullptr;
    }
  };

  class ACAutomation
  {
   public:
    Node *root;

    ACAutomation() { root = new Node(); }

    // 你有多少个匹配串，就调用多少次insert
    void insert(string str)
    {
      Node *cur = root;
      int index = 0;
      for (int i = 0; i < str.length(); i++)
      {
        index = (int) str[i];
        if (cur->nexts.find(index) == cur->nexts.end())
        {
          cur->nexts.emplace(index, new Node());
        }
        cur = cur->nexts.at(index);
      }
      cur->end++;
    }

    void build()
    {
      queue<Node *> queue;
      queue.push(root);
      Node *cur = nullptr;
      Node *cfail = nullptr;
      // bfs
      while (!queue.empty())
      {
        cur = queue.front();  // 父
        queue.pop();
        for (auto entry : cur->nexts)  // 下级所有节点
        {
          int index = entry.first;
          Node *child = entry.second;
          child->fail = root;  // 初始时先设置一个值
          cfail = cur->fail;
          while (cfail != nullptr)  // cur不是根节点
          {
            if (cfail->nexts.find(index) != cfail->nexts.end())
            {
              // 把索引为index的子节点的fail设置为索引为index的cfail的子节点
              child->fail = cfail->nexts.at(index);
              break;
            }
            cfail = cfail->fail;
          }
          queue.push(child);
        }
      }
    }

    int containNum(string str)
    {
      Node *cur = root;
      Node *follow = nullptr;
      int index = 0;
      int ans = 0;
      for (int i = 0; i < str.length(); i++)
      {
        index = (int) str[i];
        // 如果当前字符在这条路上没配出来，就随着fail方向走向下条路径
        while (cur->nexts.find(index) == cur->nexts.end() && cur != root)
        {
          cur = cur->fail;
        }
        // 1) 现在来到的路径，是可以继续匹配的
        // 2) 现在来到的节点，就是前缀树的根节点
        cur = cur->nexts.find(index) != cur->nexts.end() ? cur->nexts.at(index) : root;
        follow = cur;
        while (follow != root)
        {
          if (follow->end == -1)
          {
            break;
          }
          {
            // 不同的需求，在这一段{ }之间修改
            ans += follow->end;
            follow->end = -1;
          }
          // 不同的需求，在这一段{ }之间修改
          follow = follow->fail;
        }
      }
      return ans;
    }
  };

  static void test()
  {
    ACAutomation ac;
    ac.insert("dhe");
    ac.insert("he");
    ac.insert("c");
    ac.build();
    cout << (ac.containNum("cdhe")) << endl;
  }
};

int main()
{
  AC1::test();
  return 0;
}
