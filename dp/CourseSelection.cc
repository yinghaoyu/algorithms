#include <vector>

using namespace std;
// 选课
// 在大学里每个学生，为了达到一定的学分，必须从很多课程里选择一些课程来学习
// 在课程里有些课程必须在某些课程之前学习，如高等数学总是在其它课程之前学习
// 现在有 N 门功课，每门课有个学分，每门课有一门或没有直接先修课
// 若课程 a 是课程 b 的先修课即只有学完了课程 a，才能学习课程 b
// 一个学生要从这些课程里选择 M 门课程学习
// 问他能获得的最大学分是多少
// 测试链接 : https://www.luogu.com.cn/problem/P2014
// @sa https://www.bilibili.com/video/BV1ae411f7AC/

// 普通解法，邻接表建图 + 相对好懂的动态规划
// 几乎所有题解都是普通解法的思路，只不过优化了常数时间、做了空间压缩
// 但时间复杂度依然是O(n * 每个节点的孩子平均数量 * m的平方)
// 树型dp
class CourseSelection
{
 public:
  static const int MAXN = 301;
  int nums[MAXN];

  // 从 N 门选出 M 门能获得的最大学分
  // course[i][0] 表示第 i 门课的先修课，为0表示没有
  // courses[i][1] 表示修完能获得的学分
  int courseSelection1(int M, vector<vector<int>>& courses)
  {
    int N = courses.size();
    vector<vector<int>> graph(N);
    // 建图
    for (int i = 0; i < N; i++)
    {
      graph[courses[i][0]].push_back(i);
    }
    vector<vector<vector<int>>> dp(N, vector<vector<int>>(N, vector<int>(M, -1)));

    return f(0, graph[0].size(), M, graph, courses, dp);
  }

  // 当前来到i号节点为头的子树
  // 只在i号节点、及其i号节点下方的前j棵子树上挑选节点
  // 一共挑选k个节点，并且保证挑选的节点连成一片
  // 返回最大的累加和
  int f(int i, int j, int k, vector<vector<int>>& graph, vector<vector<int>>& courses, vector<vector<vector<int>>>& dp)
  {
    if (k == 0)
    {
      return 0;
    }
    if (j == 0 || k == 1)
    {
      return courses[i][1];
    }
    if (dp[i][j][k] != -1)
    {
      return dp[i][j][k];
    }
    // 第 j 棵树不挑节点
    int ans = f(i, j - 1, k, graph, courses, dp);
    // 在前 j-1 棵树挑 k-s 个节点，在第 j 棵树挑 s 个节点
    // 第j棵子树头节点v
    int v = graph[i][j];
    for (int s = 1; s < k; s++)
    {
      ans = std::max(ans, f(i, j - 1, k - s, graph, courses, dp) + f(v, graph[v].size(), s, graph, courses, dp));
    }
    dp[i][j][k] = ans;
    return ans;
  }

  int courseSelection2(int M, vector<vector<int>>& courses)
  {
    // TODO:
    return 0;
  }
};
