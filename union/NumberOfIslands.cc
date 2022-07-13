#include <iostream>
#include <list>
#include <unordered_map>
#include <vector>

using namespace std;
using TdArray = vector<vector<char>>;

// 本题为leetcode原题
// 测试链接：https://leetcode.com/problems/number-of-islands/

// 暴力递归
// 从(i,j)这个位置出发，把所有练成一片的'1'字符，变成0
void infect(TdArray &board, int i, int j)
{
  if (i < 0 || i == board.size() || j < 0 || j == board[0].size() || board[i][j] != '1')
  {
    return;
  }
  board[i][j] = 0;
  infect(board, i - 1, j);  // 感染左、右、下、上
  infect(board, i + 1, j);
  infect(board, i, j - 1);
  infect(board, i, j + 1);
}

int numIslands3(TdArray &board)
{
  int islands = 0;
  for (int i = 0; i < board.size(); i++)
  {
    for (int j = 0; j < board.size(); j++)
    {
      if (board[i][j] == '1')
      {
        islands++;
        infect(board, i, j);
      }
    }
  }
  return islands;
}

// hashmap
typedef struct Dot
{
} Dot;

template <typename V>
class Node
{
 public:
  V value;

  Node(V v) { value = v; }
};

template <typename V>
class UnionFind1
{
 public:
  unordered_map<V, Node<V> *> nodes;            // 所有节点
  unordered_map<Node<V> *, Node<V> *> parents;  // key节点的父亲是value
  unordered_map<Node<V> *, int> sizeMap;        // 这个根节点有多少个孩子

  UnionFind1(list<V> &values)
  {
    for (V cur : values)
    {
      Node<V> *node = new Node<V>(cur);  // 给每个元素套壳
      nodes.emplace(cur, node);
      parents.emplace(nodes.at(cur), nodes.at(cur));  // 保存指针就行
      sizeMap.emplace(nodes.at(cur), 1);
    }
  }

  // 找到cur的根节点，并压缩路径
  Node<V> *findFather(Node<V> *cur)
  {
    vector<Node<V> *> path;
    while (parents.find(cur) != parents.end() && cur != parents[cur])
    {
      path.push_back(cur);
      cur = parents.at(cur);  // 找到根节点，此处不会越界
    }
    while (!path.empty())
    {
      parents.emplace(path.back(), cur);  // 路径压缩，设置每个节点的根
      path.pop_back();
    }
    return cur;  // 返回根节点
  }

  // 吞并节点
  void unions(V a, V b)
  {
    Node<V> *aHead = findFather(nodes.at(a));
    Node<V> *bHead = findFather(nodes.at(b));
    if (aHead != bHead)  // 多的吞并少的
    {
      int aSetSize = sizeMap.at(aHead);
      int bSetSize = sizeMap.at(bHead);
      Node<V> *big = aSetSize >= bSetSize ? aHead : bHead;
      Node<V> *small = big == aHead ? bHead : aHead;
      parents.emplace(small, big);
      sizeMap.emplace(big, aSetSize + bSetSize);
      sizeMap.erase(small);
    }
  }

  int sets() { return sizeMap.size(); }
};

int numIslands1(TdArray &board)
{
  int row = board.size();
  int col = board[0].size();
  Dot dots[row][col];
  list<Dot *> dotList;
  for (int i = 0; i < row; i++)
  {
    for (int j = 0; j < col; j++)
    {
      if (board[i][j] == '1')
      {
        dotList.push_back(&dots[i][j]);  // 标记有效元素
      }
    }
  }
  UnionFind1<Dot *> uf(dotList);
  for (int j = 1; j < col; j++)  // 第0列元素
  {
    // (0,j)  (0,0)跳过了  (0,1) (0,2) (0,3)
    if (board[0][j - 1] == '1' && board[0][j] == '1')
    {
      uf.unions(&dots[0][j - 1], &dots[0][j]);
    }
  }
  for (int i = 1; i < row; i++)  // 第0行元素
  {
    if (board[i - 1][0] == '1' && board[i][0] == '1')
    {
      uf.unions(&dots[i - 1][0], &dots[i][0]);
    }
  }
  for (int i = 1; i < row; i++)  // 从左上到右下，吞并剩下的元素
  {
    for (int j = 1; j < col; j++)
    {
      if (board[i][j] == '1')
      {
        if (board[i][j - 1] == '1')  // 上
        {
          uf.unions(&dots[i][j - 1], &dots[i][j]);
        }
        if (board[i - 1][j] == '1')  // 左
        {
          uf.unions(&dots[i - 1][j], &dots[i][j]);
        }
      }
    }
  }
  return uf.sets();
}

// 数组
class UnionFind2
{
 private:
  vector<int> parent;
  vector<int> size;  // size[i]表示记录以i为根的集合有多少个元素
  vector<int> help;  // 辅助数组，用于记录路径上所有的元素
  int col;
  int number;

 public:
  UnionFind2(TdArray board)
  {
    col = board[0].size();
    number = 0;
    int row = board.size();
    int len = row * col;
    vector<int> tmp(len);
    parent = tmp;
    size = tmp;
    help = tmp;
    for (int r = 0; r < row; r++)
    {
      for (int c = 0; c < col; c++)
      {
        if (board[r][c] == '1')
        {
          int i = index(r, c);  // 二维数组转成一维数组
          parent[i] = i;        // 默认根节点是自己
          size[i] = 1;          // 以i为根的元素，默认集合元素只有一个
          number++;
        }
      }
    }
  }
  // (r,c) -> i
  int index(int r, int c) { return r * col + c; }

  // 原始位置 -> 下标
  int find(int i)
  {
    int hi = 0;
    while (i != parent[i])
    {
      help[hi++] = i;  // 记录路径上所有的元素
      i = parent[i];
    }
    for (hi--; hi >= 0; hi--)  // 路径压缩
    {
      parent[help[hi]] = i;  // 把所有元素的父亲设置为根节点
    }
    return i;  // 返回根节点
  }

  void unions(int r1, int c1, int r2, int c2)
  {
    int i1 = index(r1, c1);
    int i2 = index(r2, c2);
    int f1 = find(i1);
    int f2 = find(i2);
    if (f1 != f2)
    {
      if (size[f1] >= size[f2])  // 节点多的集合吞并节点少的
      {
        size[f1] += size[f2];
        parent[f2] = f1;
      }
      else
      {
        size[f2] += size[f1];
        parent[f1] = f2;
      }
      number--;  // 吞并后集合减1
    }
  }

  int sets() { return number; }
};

int numIslands2(TdArray &board)
{
  int row = board.size();
  int col = board[0].size();
  UnionFind2 uf(board);
  for (int j = 1; j < col; j++)  // 第0行元素
  {
    if (board[0][j - 1] == '1' && board[0][j] == '1')
    {
      uf.unions(0, j - 1, 0, j);
    }
  }
  for (int i = 1; i < row; i++)  // 第0列元素
  {
    if (board[i - 1][0] == '1' && board[i][0] == '1')
    {
      uf.unions(i - 1, 0, i, 0);
    }
  }
  for (int i = 1; i < row; i++)  // 从左上到右下
  {
    for (int j = 1; j < col; j++)
    {
      if (board[i][j] == '1')
      {
        if (board[i][j - 1] == '1')  // 上
        {
          uf.unions(i, j - 1, i, j);
        }
        if (board[i - 1][j] == '1')  // 左
        {
          uf.unions(i - 1, j, i, j);
        }
      }
    }
  }
  return uf.sets();
}
