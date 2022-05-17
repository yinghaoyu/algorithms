#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <stdbool.h>

using namespace std;

#define MAX_SEED 1024
#define HALF_SEED (MAX_SEED >> 1)

class TreeEqual
{
  public:
    class Node
    {
      public:
        int value;
        Node* left;
        Node* right;

        Node(int v)
        {
          value = v;
        }
    };

    static bool containsTree1(Node* big, Node* small)
    {
      if (small == nullptr)
      {
        return true;
      }
      if (big == nullptr)
      {
        return false;
      }
      if (isSameValueStructure(big, small))
      {
        return true;
      }
      return containsTree1(big->left, small) || containsTree1(big->right, small);
    }

    static bool isSameValueStructure(Node* head1, Node* head2)
    {
      if (head1 == nullptr && head2 != nullptr)
      {
        return false;
      }
      if (head1 != nullptr && head2 == nullptr)
      {
        return false;
      }
      if (head1 == nullptr && head2 == nullptr)
      {
        return true;
      }
      if (head1->value != head2->value)
      {
        return false;
      }
      return isSameValueStructure(head1->left, head2->left)
        && isSameValueStructure(head1->right, head2->right);
    }

    static bool containsTree2(Node* big, Node* small)
    {
      if (small == nullptr)
      {
        return true;
      }
      if (big == nullptr)
      {
        return false;
      }
      vector<string> str = preSerial(big);
      vector<string> match = preSerial(small);
      return getIndexOf(str, match) != -1;
    }

    static vector<string> preSerial(Node* head)
    {
      vector<string> ans;
      pres(head, ans);
      return ans;
    }

    static void pres(Node* head, vector<string>& ans)
    {
      if (head == nullptr)
      {
        ans.push_back("nullptr");
      }
      else
      {
        ans.push_back(std::to_string(head->value));
        pres(head->left, ans);
        pres(head->right, ans);
      }
    }

    // KMP
    static int getIndexOf(vector<string> str1, vector<string> str2)
    {
      if (str1.size() < 1 || str1.size() < str2.size())
      {
        return -1;
      }
      int x = 0;
      int y = 0;
      int* next = getNextArray(str2);
      while (x < str1.size() && y < str2.size())
      {
        if (isEqual(str1[x], str2[y]))
        {
          x++;
          y++;
        }
        else if (next[y] == -1)
        {
          x++;
        }
        else
        {
          y = next[y];
        }
      }
      return y == str2.size() ? x - y : -1;
    }

    static int* getNextArray(vector<string> ms)
    {
      if (ms.size() == 1)
      {
        return new int[1] { -1 };
      }
      int* next = new int[ms.size()];
      next[0] = -1;
      next[1] = 0;
      int i = 2;
      int cn = 0;
      while (i < ms.size())
      {
        if (isEqual(ms[i - 1], ms[cn]))
        {
          next[i++] = ++cn;
        }
        else if (cn > 0)
        {
          cn = next[cn];
        }
        else
        {
          next[i++] = 0;
        }
      }
      return next;
    }

    static bool isEqual(string a, string b)
    {
      if (a == "" && b == "")
      {
        return true;
      }
      else
      {
        if (a == "" || b == "")
        {
          return false;
        }
        else
        {
          return a.find(b) != string::npos;
        }
      }
    }

    // for test
    static Node* generateRandomBST(int maxLevel, int maxValue)
    {
      return generate(1, maxLevel, maxValue);
    }

    // for test
    static Node* generate(int level, int maxLevel, int maxValue)
    {
      if (level > maxLevel || getRandom(0, MAX_SEED) < HALF_SEED)
      {
        return nullptr;
      }
      Node* head = new Node(getRandom(0, maxValue));
      head->left = generate(level + 1, maxLevel, maxValue);
      head->right = generate(level + 1, maxLevel, maxValue);
      return head;
    }

    static int getRandom(int min, int max)
    {
      random_device seed;  // 硬件生成随机数种子
      ranlux48 engine(seed());  // 利用种子生成随机数引
      uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
      int res = distrib(engine);  // 随机数
      return res;
    }

    static void test()
    {
      int bigTreeLevel = 7;
      int smallTreeLevel = 4;
      int nodeMaxValue = 5;
      int testTimes = 100000;
      cout << "test begin" << endl;
      for (int i = 0; i < testTimes; i++)
      {
        Node* big = generateRandomBST(bigTreeLevel, nodeMaxValue);
        Node* small = generateRandomBST(smallTreeLevel, nodeMaxValue);
        bool ans1 = containsTree1(big, small);
        bool ans2 = containsTree2(big, small);
        if (ans1 != ans2)
        {
          cout << "Oops!" << endl;
        }
      }
      cout << "test finish!" << endl;
    }
};

int main()
{
  TreeEqual::test();
  return 0;
}
