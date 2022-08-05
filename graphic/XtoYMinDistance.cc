#include <iostream>
#include <queue>
#include <random>
#include <vector>

using namespace std;
using TdArray = vector<vector<int>>;

// 邻接矩阵的方式Dijkstra
// 问题描述：
// 一天Peiger捧着一本世界地图在看，突然他拿起笔
// 将他最爱的那些城市标记出来，并且随机的将这些城市中的某些用线段两两连接起来。▫
// Peiger量出了每条线段的长度，现在Peiger想知道在这些线段组成的图中任意两个城市之间的最短距离是多少。
//
// 输入
// 输入包含多组测试数据。
// 每组输入第一行为两个正整数n（n<=10）和m（m<=n*(n-1)/2），n表示城市个数，m表示线段个数。
// 接下来m行，每行输入三个整数a，b和l，表示a市与b市之间存在一条线段，线段长度为l。（a与b不同）
// 每组最后一行输入两个整数x和y，表示问题：x市与y市之间的最短距离是多少。（x与y不同）
// 城市标号为1~n，l<=20。
//
// 输出
// 对于每组输入，输出x市与y市之间的最短距离，如果x市与y市之间非连通，则输出“No path”。
//
// 样例输入
// 4 4
// 1 2 4
// 1 3 1
// 1 4 1
// 2 3 1
// 2 4
// 样例输出
// 3

class XtoYMinDistance
{
 public:
  // 暴力方法
  // dfs尝试所有情况
  // 没有优化，就是纯暴力
  static int minDistance1(int n, int m, TdArray &roads, int x, int y)
  {
    // 第一步生成邻接矩阵
    TdArray map(n + 1, vector<int>(n + 1));
    for (int i = 0; i <= n; i++)
    {
      for (int j = 0; j <= n; j++)
      {
        map[i][j] = INT32_MAX;
      }
    }
    for (auto road : roads)
    {
      // 根据题意，这里是无向图
      map[road[0]][road[1]] = std::min(map[road[0]][road[1]], road[2]);
      map[road[1]][road[0]] = std::min(map[road[1]][road[0]], road[2]);
    }
    vector<bool> visited(n + 1);
    return process(x, y, n, map, visited);
  }

  // 当前来到的城市是cur，最终目的地是aim，一共有1~n这些城市
  // 所有城市之间的距离都在map里
  // 之前已经走过了哪些城市都记录在了visited里面，请不要重复经过
  // 返回从cur到aim所有可能的路里，最小距离是多少
  static int process(int cur, int aim, int n, TdArray &map, vector<bool> &visited)
  {
    if (visited[cur])
    {
      return INT32_MAX;
    }
    if (cur == aim)
    {
      return 0;
    }
    visited[cur] = true;
    int ans = INT32_MAX;
    for (int next = 1; next <= n; next++)
    {
      if (next != cur && map[cur][next] != INT32_MAX)
      {
        int rest = process(next, aim, n, map, visited);
        if (rest != INT32_MAX)
        {
          ans = std::min(ans, map[cur][next] + rest);
        }
      }
    }
    visited[cur] = false;
    return ans;
  }

  // Dijkstra的解
  static int minDistance2(int n, int m, TdArray &roads, int x, int y)
  {
    // 第一步生成邻接矩阵
    TdArray map(n + 1, vector<int>(n + 1));
    for (int i = 0; i <= n; i++)
    {
      for (int j = 0; j <= n; j++)
      {
        map[i][j] = INT32_MAX;
      }
    }
    for (auto road : roads)
    {
      map[road[0]][road[1]] = std::min(map[road[0]][road[1]], road[2]);
      map[road[1]][road[0]] = std::min(map[road[1]][road[0]], road[2]);
    }
    // computed[i] = true，表示从源出发点到i这个城市，已经计算出最短距离了
    // computed[i] = false，表示从源出发点到i这个城市，还没有计算出最短距离
    vector<bool> computed(n + 1);
    // 距离小根堆
    priority_queue<Node *, vector<Node *>, cmp> heap;
    heap.emplace(new Node(x, 0));
    while (!heap.empty())
    {
      Node *cur = heap.top();
      heap.pop();
      if (computed[cur->city])
      {
        continue;
      }
      if (cur->city == y)
      {
        return cur->pathSum;
      }
      computed[cur->city] = true;
      for (int next = 1; next <= n; next++)
      {
        if (next != cur->city && map[cur->city][next] != INT32_MAX && !computed[next])
        {
          // 因为从小根堆里面选出来的边一定是最小的，所有这里不需要取最小值
          heap.emplace(new Node(next, cur->pathSum + map[cur->city][next]));
        }
      }
    }
    return INT32_MAX;
  }

  // 当前来到的Node，注意这不是城市的意思，这是就是一个普通的封装类
  // Node封装了，当前来到的城市是什么，以及，从源出发点到这个城市的路径和是多少？
  class Node
  {
   public:
    // 当前来到的城市编号
    int city;
    // 从源出发点到这个城市的路径和
    int pathSum;

    Node(int c, int p)
    {
      city = c;
      pathSum = p;
    }
  };

  class cmp
  {
   public:
    bool operator()(const Node *a, const Node *b) { return a->pathSum > b->pathSum; }
  };
  // 为了测试
  // 城市1~n
  // 随机生成m条道路
  // 每一条路的距离，在1~v之间
  static TdArray randomRoads(int n, int m, int v)
  {
    TdArray roads(m, vector<int>(3));
    for (int i = 0; i < m; i++)
    {
      int from = getRandom(1, n);
      int to = getRandom(1, n);
      int distance = getRandom(1, v);
      roads[i] = {from, to, distance};
    }
    return roads;
  }

  static int getRandom(int min, int max)
  {
    random_device seed;                            // 硬件生成随机数种子
    ranlux48 engine(seed());                       // 利用种子生成随机数引
    uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
    int res = distrib(engine);                     // 随机数
    return res;
  }

  // 为了测试
  static void test()
  {
    // 城市数量n，下标从1开始，不从0开始
    int n = 4;
    // 边的数量m，m的值不能大于n * (n-1)
    int m = 4;
    // 所的路有m条
    // [a,b,c]表示a和b之间有路，距离为3，根据题意，本题中的边都是无向边
    // 假设有两条路
    // [1,3,7]，这条路是从1到3，距离是7
    // [1,3,4]，这条路是从1到3，距离是4
    // 那么应该忽略[1,3,7]，因为[1,3,4]比它好
    TdArray roads(m, vector<int>(3));
    roads[0] = {1, 2, 4};
    roads[1] = {1, 3, 1};
    roads[2] = {1, 4, 1};
    roads[3] = {2, 3, 1};
    // 求从x到y的最短距离是多少，x和y应该在[1,n]之间
    int x = 2;
    int y = 4;
    // 暴力方法的解
    cout << minDistance1(n, m, roads, x, y) << endl;

    // Dijkstra的解
    cout << minDistance2(n, m, roads, x, y) << endl;

    // 下面开始随机验证
    int cityMaxSize = 12;
    int pathMax = 30;
    int testTimes = 2000;
    cout << "测试开始" << endl;
    for (int i = 0; i < testTimes; i++)
    {
      n = getRandom(2, cityMaxSize);
      m = getRandom(1, n * (n - 1) / 2);
      roads = randomRoads(n, m, pathMax);
      x = getRandom(1, n);
      y = getRandom(1, n);
      int ans1 = minDistance1(n, m, roads, x, y);
      int ans2 = minDistance2(n, m, roads, x, y);
      if (ans1 != ans2)
      {
        cout << "出错了！" << endl;
      }
    }
    cout << "测试结束" << endl;
  }
};

int main()
{
  XtoYMinDistance::test();
  return 0;
}
