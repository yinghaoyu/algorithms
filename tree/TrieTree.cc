#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <string.h>

using namespace std;

class TrieTree
{
  public:
    class Node
    {
      public:
        int pass;  // 标记该节点有多少个相同前缀的节点
        int end;  //标记该节点加入过多少次，或者说有多少个以该节点结尾的单词
        unordered_map<int, Node*> nexts;

        Node()
        {
          pass = 0;
          end = 0;
        }
    };

    class Trie
    {
      public:
        Node* root;

        Trie()
        {
          root = new Node();
        }

        void insert(string word)
        {
          if (word == "")
          {
            return;
          }
          const char* chs = word.c_str();
          Node* node = root;
          node->pass++;
          int index = 0;
          int len = strlen(chs);
          for (int i = 0; i < len; i++)
          {
            // 从左往右遍历字符
            index = (int)chs[i]; // 由字符，对应成走向哪条路
            if (node->nexts.find(index) == node->nexts.end())
            {
              node->nexts.emplace(index, new Node());
            }
            node = node->nexts.at(index);
            node->pass++;
          }
          node->end++;
        }

        void del(string word)
        {
          if (search(word) != 0)
          {
            const char* chs = word.c_str();
            Node* node = root;
            node->pass--;
            int index = 0;
            int len = strlen(chs);
            for (int i = 0; i < len; i++)
            {
              index = (int) chs[i];
              if (--node->nexts.at(index)->pass == 0)
              {
                node->nexts.erase(index);
                return;
              }
              node = node->nexts.at(index);
            }
            node->end--;
          }
        }

        // word这个单词之前加入过几次
        int search(string word)
        {
          if (word == "")
          {
            return 0;
          }
          const char* chs = word.c_str();
          Node* node = root;
          int index = 0;
          int len = strlen(chs);
          for (int i = 0; i < len; i++)
          {
            index = (int) chs[i];
            if (node->nexts.find(index) == node->nexts.end()) 
            {
              return 0;
            }
            node = node->nexts.at(index);
          }
          return node->end;
        }

        // 所有加入的字符串中，有几个是以pre这个字符串作为前缀的
        int prefixNumber(string pre)
        {
          if (pre == "")
          {
            return 0;
          }
          const char* chs = pre.c_str();
          Node* node = root;
          int index = 0;
          int len = strlen(chs);
          for (int i = 0; i < len; i++)
          {
            index = (int) chs[i];
            if (node->nexts.find(index) == node->nexts.end())
            {
              return 0;
            }
            node = node->nexts.at(index);
          }
          return node->pass;
        }
    };

    class Right
    {
      public:
        unordered_map<string, int> box;

        void insert(string word)
        {
          if(box.find(word) == box.end())
          {
            box.emplace(word, 1);
          }
          else
          {
            box[word]++;
          }
        }

        void del (string word)
        {
          if(box.find(word) != box.end())
          {
            if(box.at(word) == 1)
            {
              box.erase(word);
            }
            else
            {
              box[word]--;
            }
          }
        }

        int search(string word)
        {
          if(box.find(word) == box.end())
          {
            return 0;
          }
          else
          {
            return box.at(word);
          }
        }

        int prefixNumber(string pre)
        {
          int count = 0;
          for(auto cur : box)
          {
            if(cur.first.find(pre) == 0)  // pre是开始字符
            {
              count += box.at(cur.first);
            }
          }
          return count;
        }
    };

    static int getRandom(int min, int max)
    {
      random_device seed;  // 硬件生成随机数种子
      ranlux48 engine(seed());  // 利用种子生成随机数引
      uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
      int res = distrib(engine);  // 随机数
      return res;
    }

    static string generateRandomString(int strLen)
    {
      int len = getRandom(1, strLen);
      char* ans = new char[len];
      for (int i = 0; i < len; i++)
      {
        int value = (int) getRandom(0, 5);
        ans[i] = (char) (97 + value);
      }
      return string(ans);
    }

    static string* generateRandomStringArray(int arrLen, int strLen, int* len)
    {
      *len = getRandom(1, arrLen);
      string* ans = new string[*len];
      for (int i = 0; i < *len; i++)
      {
        ans[i] = generateRandomString(strLen);
      }
      return ans;
    }
};

int main()
{
  int arrLen = 100;
  int strLen = 20;
  int testTimes = 1000;
  int len = 0;
  for (int i = 0; i < testTimes; i++)
  {
    string* arr = TrieTree::generateRandomStringArray(arrLen, strLen, &len);
    TrieTree::Trie trie;
    TrieTree::Right right;
    for (int j = 0; j < len; j++)
    {
      double decide = double (TrieTree::getRandom(0, 1023) / 1024.0);
      if (decide < 0.25)
      {
        trie.insert(arr[j]);
        right.insert(arr[j]);
      }
      else if (decide < 0.5)
      {
        trie.del(arr[j]);
        right.del(arr[j]);
      }
      else if (decide < 0.75)
      {
        int ans1 = trie.search(arr[j]);
        int ans2 = right.search(arr[j]);
        if (ans1 != ans2)
        {
          cout << "Oops!" << endl;
        }
      }
      else
      {
        int ans1 = trie.prefixNumber(arr[j]);
        int ans2 = right.prefixNumber(arr[j]);
        if (ans1 != ans2)
        {
          cout << "Oops!" << endl;
        }
      }
    }
  }
  cout << "finish!" << endl;
  return 0;
}
