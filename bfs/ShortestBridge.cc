#include <iostream>
#include <vector>

using namespace std;
using TdArray = vector<vector<int>>;

// 问题描述：
// 在给定的二维二进制数组 A 中，存在两座岛。（岛是由四面相连的 1 形成的一个最大组。）
// 现在，我们可以将 0 变为 1，以使两座岛连接起来，变成一座岛。
// 返回必须翻转的 0 的最小数目。（可以保证答案至少是 1 。）

// 本题测试链接 : https://leetcode.com/problems/shortest-bridge/

class ShortestBridge
{
 public:
  static int shortestBridge(TdArray &m)
  {
    int N = m.size();
    int M = m[0].size();
    int all = N * M;
    int island = 0;
    vector<int> curs(all);
    vector<int> nexts(all);
    TdArray records(2, vector<int>(all));
    for (int i = 0; i < N; i++)
    {
      for (int j = 0; j < M; j++)
      {
        if (m[i][j] == 1)
        {
          // 当前位置发现了1！
          // 把这一片的1，都变成2，同时，抓上来了，这一片1组成的初始队列
          // curs, 把这一片的1到自己的距离，都设置成1了，records
          int queueSize = infect(m, i, j, N, M, curs, 0, records[island]);
          int V = 1;
          while (queueSize != 0)
          {
            V++;
            // curs里面的点，上下左右，records[点]==0， nexts
            queueSize = bfs(N, M, all, V, curs, queueSize, nexts, records[island]);
            vector<int> tmp = curs;
            curs = nexts;
            nexts = tmp;
          }
          island++;
        }
      }
    }
    int min = INT32_MAX;
    for (int i = 0; i < all; i++)
    {
      min = std::min(min, records[0][i] + records[1][i]);
    }
    return min - 3;
  }

  // 当前来到m[i][j] , 总行数是N，总列数是M
  // m[i][j]感染出去(找到这一片岛所有的1),把每一个1的坐标，放入到int[] curs队列！
  // 1 (a,b) -> curs[index++] = (a * M + b)
  // 1 (c,d) -> curs[index++] = (c * M + d)
  // 二维已经变成一维了， 1 (a,b) -> a * M + b
  // 设置距离record[a * M +b ] = 1
  static int infect(TdArray &m, int i, int j, int N, int M, vector<int> &curs, int index, vector<int> &record)
  {
    if (i < 0 || i == N || j < 0 || j == M || m[i][j] != 1)
    {
      return index;
    }
    // m[i][j] 不越界，且m[i][j] == 1
    m[i][j] = 2;
    int p = i * M + j;
    record[p] = 1;
    // 收集到不同的1
    curs[index++] = p;
    index = infect(m, i - 1, j, N, M, curs, index, record);
    index = infect(m, i + 1, j, N, M, curs, index, record);
    index = infect(m, i, j - 1, N, M, curs, index, record);
    index = infect(m, i, j + 1, N, M, curs, index, record);
    return index;
  }

  // 二维原始矩阵中，N总行数，M总列数
  // all 总 all = N * M
  // V 要生成的是第几层 curs V-1 nexts V
  // record里面拿距离
  static int bfs(int N, int M, int all, int V, vector<int> &curs, int size, vector<int> &nexts, vector<int> &record)
  {
    int nexti = 0;  // 我要生成的下一层队列成长到哪了？
    for (int i = 0; i < size; i++)
    {
      // curs[i] -> 一个位置
      int up = curs[i] < M ? -1 : curs[i] - M;
      int down = curs[i] + M >= all ? -1 : curs[i] + M;
      int left = curs[i] % M == 0 ? -1 : curs[i] - 1;
      int right = curs[i] % M == M - 1 ? -1 : curs[i] + 1;
      if (up != -1 && record[up] == 0)
      {
        record[up] = V;
        nexts[nexti++] = up;
      }
      if (down != -1 && record[down] == 0)
      {
        record[down] = V;
        nexts[nexti++] = down;
      }
      if (left != -1 && record[left] == 0)
      {
        record[left] = V;
        nexts[nexti++] = left;
      }
      if (right != -1 && record[right] == 0)
      {
        record[right] = V;
        nexts[nexti++] = right;
      }
    }
    return nexti;
  }
};

int main()
{
  return 0;
}
