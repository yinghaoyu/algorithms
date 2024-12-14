#include <algorithm>
#include <iostream>
#include <vector>

// 克鲁斯卡尔最小生成树
// 算法思想：使用并查集
// （1）总是从权值最小的边开始找，依次找权值依次变大的边（权值相等任选其一）
// （2）如果当前的边进入最小生成树的集合中不会形成环，就要当前边；否则舍弃
// （3）找完所有边之后，最小生成树的集合也就得到了

using namespace std;

class Kruskal
{
 private:
  static const int MAXN = 5001;

  static const int MAXM = 200001;

  vector<int> father = vector<int>(MAXM);

  // u, v, w
  vector<vector<int>> edges = vector<vector<int>>(MAXM, vector<int>(3));

  int n, m;

  void build()
  {
    for (int i = 1; i <= n; i++)
    {
      father[i] = i;
    }
  }

  int find(int i)
  {
    if (i != father[i])
    {
      father[i] = find(father[i]);
    }
    return father[i];
  }

  // 如果x和y本来就是一个集合，返回false
  // 如果x和y不是一个集合，合并之后返回true
  bool unions(int x, int y)
  {
    int fx = find(x);
    int fy = find(y);
    if (fx != fy)
    {
      father[fx] = fy;
      return true;
    }
    else
    {
      return false;
    }
  }

  void test()
  {
    // 根据边的大小排序
    std::sort(edges.begin(), edges.begin() + m, [](auto& a, auto& b) { return a[2] < b[2]; });
    int ans = 0;
    int edgeCnt = 0;
    for (auto& edge : edges)
    {
      // 依次从小到大筛选边，看看是否在同一集合内
      if (unions(edge[0], edge[1]))
      {
        // 只要不在集合内的邻接点，就可以选择这条边
        edgeCnt++;
        ans += edge[2];
      }
    }
    if (edgeCnt == n - 1)
    {
      // 最小生成树一定是n-1条边
      cout << edgeCnt << endl;
    }
    else
    {
      cout << "orz" << endl;
    }
  }
};
