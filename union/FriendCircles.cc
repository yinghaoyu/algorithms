#include <iostream>
#include <vector>

using namespace std;

using TdArray = vector<vector<int>>;

// 测试链接：https://leetcode.com/problems/friend-circles/
class UnionFind
{
 private:
  // parent[i] = k ： i的父亲是k
  vector<int> parent;
  // size[i] = k ： 如果i是代表节点，size[i]才有意义，否则无意义
  // i所在的集合大小是多少
  vector<int> size;
  // 辅助结构，用于记录路径上的节点，压缩路径
  vector<int> help;
  // 一共有多少个集合
  int number;

 public:
  UnionFind(int N)
  {
    vector<int> tmp(N);
    parent = tmp;
    size = tmp;
    help = tmp;
    number = N;
    for (int i = 0; i < N; i++)
    {
      parent[i] = i;  // 初始时i的父亲是自己
      size[i] = 1;    // 只有一个节点
    }
  }

  // 从i开始一直往上，往上到不能再往上，代表节点，返回
  // 这个过程要做路径压缩
  int find(int i)
  {
    int hi = 0;
    while (i != parent[i])
    {
      help[hi++] = i;  // 标记所有不同编号的节点
      i = parent[i];
    }
    for (hi--; hi >= 0; hi--)
    {
      parent[help[hi]] = i;  // 把所有节点的父亲标记为根节点
    }
    return i;  // 返回这个节点的根节点
  }

  void unions(int i, int j)
  {
    int f1 = find(i);  // 节点i的根节点
    int f2 = find(j);  // 节点j的根节点
    if (f1 != f2)
    {
      if (size[f1] >= size[f2])
      {
        // 多的吞并少的
        size[f1] += size[f2];
        parent[f2] = f1;
      }
      else
      {
        size[f2] += size[f1];
        parent[f1] = f2;
      }
      number--;  // 吞并完集合减少1个
    }
  }

  int sets() { return number; }
};

int findCircleNum(int **M, int N)
{
  // {0} {1} {2} {N-1}
  UnionFind unionFind(N);
  for (int i = 0; i < N; i++)
  {
    for (int j = i + 1; j < N; j++)
    {
      if (M[i][j] == 1)
      {  // i和j互相认识
        unionFind.unions(i, j);
      }
    }
  }
  return unionFind.sets();
}
