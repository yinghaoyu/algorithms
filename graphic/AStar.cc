#include <math.h>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <queue>
#include <random>
#include <vector>

using namespace std;

// A*算法模版
// A*算法的要求，预估函数 f(x) 得到的距离应该 <= 实际的距离
class AStarAlgorithm
{
 public:
  // 0:上，1:右，2:下，3:左
  vector<int> move = {-1, 0, 1, 0, -1};

  class cmp
  {
   public:
    bool operator()(vector<int>& a, vector<int>& b) { return a[2] > b[2]; }
  };

  // Dijkstra算法
  // grid[i][j] == 0 代表障碍
  // grid[i][j] == 1 代表道路
  // 只能走上、下、左、右，不包括斜线方向
  // 返回从(startX, startY)到(targetX, targetY)的最短距离
  int minDistance1(vector<vector<int>>& grid, int startX, int startY, int targetX, int targetY)
  {
    if (grid[startX][startY] == 0 || grid[targetX][targetY] == 0)
    {
      return -1;
    }
    int n = grid.size();
    int m = grid[0].size();
    vector<vector<int>> distance = vector<vector<int>>(n, vector<int>(m, INT32_MAX));
    distance[startX][startY] = 1;
    vector<vector<bool>> visited(n, vector<bool>(m));
    priority_queue<vector<int>, vector<vector<int>>, cmp> heap;
    // 0 : 行
    // 1 : 列
    // 2 : 从源点出发到达当前点的距离
    heap.push({startX, startY, 1});
    while (!heap.empty())
    {
      auto cur = heap.top();
      int x = cur[0];
      int y = cur[1];
      heap.pop();
      if (visited[x][y])
      {
        continue;
      }
      visited[x][y] = true;
      if (x == targetX && y == targetY)
      {
        return distance[x][y];
      }
      for (int i = 0, nx, ny; i < 4; i++)
      {
        nx = x + move[i];
        ny = y + move[i + 1];
        if (nx >= 0 && nx < n && ny >= 0 && ny < m && grid[nx][ny] == 1 && !visited[nx][ny] && distance[x][y] + 1 < distance[nx][ny])
        {
          distance[nx][ny] = distance[x][y] + 1;
          heap.push({nx, ny, distance[x][y] + 1});
        }
      }
    }
    return -1;
  }

  // A*算法
  // grid[i][j] == 0 代表障碍
  // grid[i][j] == 1 代表道路
  // 只能走上、下、左、右，不包括斜线方向
  // 返回从(startX, startY)到(targetX, targetY)的最短距离
  int minDistance2(vector<vector<int>>& grid, int startX, int startY, int targetX, int targetY)
  {
    if (grid[startX][startY] == 0 || grid[targetX][targetY] == 0)
    {
      return -1;
    }
    int n = grid.size();
    int m = grid[0].size();
    vector<vector<int>> distance = vector<vector<int>>(n, vector<int>(m, INT32_MAX));
    distance[startX][startY] = 1;
    vector<vector<bool>> visited(n, vector<bool>(m));
    // 0 : 行
    // 1 : 列
    // 2 : 从源点出发到达当前点的距离 + 当前点到终点的预估距离
    priority_queue<vector<int>, vector<vector<int>>, cmp> heap;
    heap.push({startX, startY, 1 + f1(startX, startY, targetX, targetY)});
    while (!heap.empty())
    {
      auto cur = heap.top();
      heap.pop();
      int x = cur[0];
      int y = cur[1];
      if (visited[x][y])
      {
        continue;
      }
      visited[x][y] = true;
      if (x == targetX && y == targetY)
      {
        return distance[x][y];
      }
      for (int i = 0, nx, ny; i < 4; i++)
      {
        nx = x + move[i];
        ny = y + move[i + 1];
        if (nx >= 0 && nx < n && ny >= 0 && ny < m && grid[nx][ny] == 1 && !visited[nx][ny] && distance[x][y] + 1 < distance[nx][ny])
        {
          distance[nx][ny] = distance[x][y] + 1;
          heap.push({nx, ny, distance[x][y] + 1 + f1(nx, ny, targetX, targetY)});
        }
      }
    }
    return -1;
  }

  // 曼哈顿距离
  int f1(int x, int y, int targetX, int targetY) { return (std::abs(targetX - x) + std::abs(targetY - y)); }

  // 对角线距离
  int f2(int x, int y, int targetX, int targetY) { return std::max(std::abs(targetX - x), std::abs(targetY - y)); }

  // 欧式距离
  double f3(int x, int y, int targetX, int targetY) { return std::sqrt(std::pow(targetX - x, 2) + std::pow(targetY - y, 2)); }

  int getRandom(int min, int max)
  {
    random_device seed;                            // 硬件生成随机数种子
    ranlux48 engine(seed());                       // 利用种子生成随机数引
    uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
    int res = distrib(engine);                     // 随机数
    return res;
  }

  // 为了测试
  vector<vector<int>> randomGrid(int n)
  {
    vector<vector<int>> grid(n, vector<int>(n));
    for (int i = 0; i < n; i++)
    {
      for (int j = 0; j < n; j++)
      {
        if (getRandom(0, 100) < 30)
        {
          // 每个格子有30%概率是0
          grid[i][j] = 0;
        }
        else
        {
          // 每个格子有70%概率是1
          grid[i][j] = 1;
        }
      }
    }
    return grid;
  }

  // 为了测试
  void test()
  {
    int len = 100;
    int testTime = 10000;
    std::cout << "功能测试开始" << std::endl;
    for (int i = 0; i < testTime; i++)
    {
      int n = getRandom(0, len) + 2;
      vector<vector<int>> grid = randomGrid(n);
      int startX = getRandom(0, n - 1);
      int startY = getRandom(0, n - 1);
      int targetX = getRandom(0, n - 1);
      int targetY = getRandom(0, n - 1);
      int ans1 = minDistance1(grid, startX, startY, targetX, targetY);
      int ans2 = minDistance2(grid, startX, startY, targetX, targetY);
      if (ans1 != ans2)
      {
        std::cout << ans1 << " " << ans2 << "出错了!" << std::endl;
      }
    }
    std::cout << "功能测试结束" << std::endl;

    std::cout << "性能测试开始" << std::endl;
    vector<vector<int>> grid = randomGrid(4000);
    int startX = 0;
    int startY = 0;
    int targetX = 3900;
    int targetY = 3900;
    auto start = chrono::high_resolution_clock::now();
    int ans1 = minDistance1(grid, startX, startY, targetX, targetY);
    auto end = chrono::high_resolution_clock::now();
    auto duration = end - start;
    std::cout << "运行dijskra算法结果: " << ans1 << ", 运行时间(毫秒) : " << duration.count() << std::endl;
    start = chrono::high_resolution_clock::now();
    int ans2 = minDistance2(grid, startX, startY, targetX, targetY);
    end = chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "运行A*算法结果: " << ans2 << ", 运行时间(毫秒) : " << duration.count() << std::endl;
    std::cout << "性能测试结束" << std::endl;
  }
};

int main()
{
  AStarAlgorithm s;
  s.test();
  return 0;
}
