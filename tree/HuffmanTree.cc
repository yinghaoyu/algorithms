#include <iostream>
#include <random>
#include <unordered_map>
#include <queue>
#include <string>
#include <string.h>

using namespace std;

// 本文件不牵扯任何byte类型的转化
// 怎么转byte自己来，我只负责huffman算法本身的正确实现
// 字符串为空的时候，自己处理边界吧
// 实现的代码通过了大样本随机测试的对数器
// 可以从main函数的内容开始看起
class HuffmanTree
{
  public:

    // 根据str, 生成词频统计表
    static unordered_map<char, int> countMap(string str)
    {
      unordered_map<char, int> ans;
      for (char cha : str)
      {
        if (ans.find(cha) == ans.end())
        {
          ans.emplace(cha, 1);
        }
        else
        {
          ans[cha]++;
        }
      }
      return ans;
    }

    class Node
    {
      public:
        int count;
        Node* left;
        Node* right;

        Node()
        {
          count = 0;
          left = nullptr;
          right = nullptr;
        }

        Node(int c)
        {
          count = c;
          left = nullptr;
          right = nullptr;
        }

        bool operator<(const Node& other) const  // 这个重载只针对priority_queue<Node>有效
        {
          return this->count < other.count;
        }
    };

    class cmp
    {
      public:
      bool operator()(const Node* a, const Node* b)  // 针对priority_queue<Node*>
      {
        return a->count > b->count;
      }
    };

    // 根据由文章生成词频表countMap，生成哈夫曼编码表
    // key : 字符
    // value: 该字符编码后的二进制形式
    // 比如，频率表 A：60, B:45, C:13 D:69 E:14 F:5 G:3
    // A 10
    // B 01
    // C 0011
    // D 11
    // E 000
    // F 00101
    // G 00100
    static unordered_map<char, string> huffmanForm(unordered_map<char, int>& countMap)
    {
      unordered_map<char, string> ans; // 生成huffman编码表
      if (countMap.size() == 1)
      {
        for (auto entry : countMap)
        {
          ans.emplace(entry.first, "0");
        }
        return ans;
      }
      unordered_map<Node*, char> nodes;
      priority_queue<Node*, vector<Node*>, cmp> heap;  // 小根堆，需要重载Node*的比较
      // Node的value存词频，char 是该词
      for (auto entry : countMap)
      {
        Node* cur = new Node(entry.second);
        char cha = entry.first;
        nodes.emplace(cur, cha);
        heap.emplace(cur);
      }
      // 根据由词频生成huffman树
      // 每次从堆取词频最小的两个节点，相加后加入堆中
      while (heap.size() != 1)
      {
        Node* a = heap.top();
        heap.pop();
        Node* b = heap.top();
        heap.pop();
        Node* h = new Node(a->count + b->count);
        h->left = a;
        h->right = b;
        heap.emplace(h);
      }
      Node* head = heap.top();
      heap.pop();
      // 根据huffman树生成huffman编码
      fillForm(head, "", nodes, ans);
      return ans;
    }

    // 从根节点开始递归生成huffman编码
    static void fillForm(Node* head,
                          string pre,
                          unordered_map<Node*, char>& nodes,
                          unordered_map<char, string>& ans)
    {
      if (nodes.find(head) != nodes.end())
      {
        ans.emplace(nodes.at(head), pre);
      }
      else
      {
        fillForm(head->left, pre + "0", nodes, ans);
        fillForm(head->right, pre + "1", nodes, ans);
      }
    }

    // 原始字符串str，根据哈夫曼编码表，转译成哈夫曼编码返回
    static string huffmanEncode(string str, unordered_map<char, string>& huffmanForm)
    {
      string ans;
      for (char cha : str)
      {
        ans.append(huffmanForm.at(cha));
      }
      return ans;
    }

    // 原始字符串的哈夫曼编码huffmanEncode，根据哈夫曼编码表，还原成原始字符串
    static string huffmanDecode(string huffmanEncode,
        unordered_map<char,
        string>& huffmanForm)
    {
      TrieNode* root = createTrie(huffmanForm);  // 得到字典树
      TrieNode* cur = root;
      string ans;
      for (int i = 0; i < huffmanEncode.length(); i++)
      {
        int index = huffmanEncode[i] == '0' ? 0 : 1;
        cur = cur->nexts[index];
        // 直到走到字典树叶子结点，才有记录的实际char
        if (cur->nexts.find(0) == cur->nexts.end()
            && cur->nexts.find(1) == cur->nexts.end())
        {
          ans.push_back(cur->value);
          cur = root;  // 复位，从根节点开始
        }
      }
      return ans;
    }

    class TrieNode
    {
      public:
        char value;
        unordered_map<int, TrieNode*> nexts;
        TrieNode()
        {
          value = 0;
        }
    };

    // 根据huffman表生成字典树
    static TrieNode* createTrie(unordered_map<char, string>& huffmanForm)
    {
      TrieNode* root = new TrieNode();  // 字典树至少有一个节点
      for (auto entry : huffmanForm)
      {
        string path = huffmanForm.at(entry.first);  // 每个字符对应的huffman编码
        TrieNode* cur = root;
        for (int i = 0; i < path.length(); i++)  // 根据huffman编码生成字典树
        {
          int index = path[i] == '0' ? 0 : 1;  // 二插字典树只有0、1
          if (cur->nexts.find(index) == cur->nexts.end())
          {
            cur->nexts[index] = new TrieNode();
          }
          cur = cur->nexts[index];
        }
        cur->value = entry.first;  // 只需要在叶子节点记录char
      }
      return root;
    }

    // 为了测试
    static string randomNumberstring(int len, int range)
    {
      string str;
      for (int i = 0; i < len; i++)
      {
        str.push_back((char)(getRandom(0, range) + 'a'));
      }
      return str;
    }

    static int getRandom(int min, int max)
    {
      random_device seed;  // 硬件生成随机数种子
      ranlux48 engine(seed());  // 利用种子生成随机数引
      uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
      int res = distrib(engine);  // 随机数
      return res;
    }

    // 为了测试
    static void test()
    {
      // 根据词频表生成哈夫曼编码表
      unordered_map<char, int> map;
      map.emplace('A', 60);
      map.emplace('B', 45);
      map.emplace('C', 13);
      map.emplace('D', 69);
      map.emplace('E', 14);
      map.emplace('F', 5);
      map.emplace('G', 3);
      unordered_map<char, string> hfForm = huffmanForm(map);
      for (auto entry : hfForm)
      {
        cout << entry.first << " : " << entry.second << endl;
      }
      cout << "====================" << endl;
      // str是原始字符串
      string str = "CBBBAABBACAABDDEFBA";
      cout << str << endl;
      // countMap是根据str建立的词频表
      unordered_map<char, int> cMap = countMap(str);
      // hf是根据countMap生成的哈夫曼编码表
      unordered_map<char, string> hf = huffmanForm(cMap);
      // huffmanEncode是原始字符串转译后的哈夫曼编码
      string hfEncode = huffmanEncode(str, hf);
      cout << hfEncode << endl;
      // huffmanDecode是哈夫曼编码还原成的原始字符串
      string hfDecode = huffmanDecode(hfEncode, hf);
      cout << hfDecode << endl;
      cout << "====================" << endl;
      cout << "大样本随机测试开始" << endl;
      // 字符串最大长度
      int len = 500;
      // 所含字符种类
      int range = 26;
      // 随机测试进行的次数
      int testTime = 10000;
      for (int i = 0; i < testTime; i++)
      {
        int N = getRandom(1, len);
        string test = randomNumberstring(N, range);
        unordered_map<char, int> counts = countMap(test);
        unordered_map<char, string> form = huffmanForm(counts);
        string encode = huffmanEncode(test, form);
        string decode = huffmanDecode(encode, form);
        if (test != decode)
        {
          cout << test << endl;
          cout << encode << endl;
          cout << decode << endl;
          cout << "出错了!" << endl;
        }
      }
      cout << "大样本随机测试结束" << endl;
    }
};

int main()
{
  HuffmanTree::test();
  return 0;
}
