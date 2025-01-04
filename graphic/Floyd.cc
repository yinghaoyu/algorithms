#include <cstdint>
#include <vector>

using namespace std;

// Floyd算法模版（洛谷）
// 测试链接 : https://www.luogu.com.cn/problem/P2910

class Floyd
{
 public:
  static const int MAXN = 101;

  static const int MAXM = 10001;

  vector<int> path = vector<int>(MAXM);

  vector<vector<int>> distance = vector<vector<int>>(MAXN, vector<int>(MAXN));

  int n, m, ans;

  // 初始时设置任意两点之间的最短距离为无穷大，表示任何路不存在
  void build()
  {
    for (int i = 0; i < n; i++)
    {
      for (int j = 0; j < n; j++)
      {
        distance[i][j] = INT32_MAX;
      }
    }
  }

  void floyd()
  {
    // O(N^3)的过程
    // 枚举每个跳板
    // 注意，跳板要最先枚举！跳板要最先枚举！跳板要最先枚举！
    for (int bridge = 0; bridge < n; bridge++)
    {  // 跳板
      for (int i = 0; i < n; i++)
      {
        for (int j = 0; j < n; j++)
        {
          // i -> .....bridge .... -> j
          // distance[i][j]能不能缩短
          // distance[i][j] = min ( distance[i][j] , distance[i][bridge] + distance[bridge][j])
          if (distance[i][bridge] != INT32_MAX && distance[bridge][j] != INT32_MAX && distance[i][j] > distance[i][bridge] + distance[bridge][j])
          {
            distance[i][j] = distance[i][bridge] + distance[bridge][j];
          }
        }
      }
    }
  }
};
