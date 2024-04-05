#include <algorithm>
#include <vector>

using namespace std;

// @sa 双向广搜 https://www.bilibili.com/video/BV11w411y79P/

// 牛牛的背包问题 & 世界冰球锦标赛
// 牛牛准备参加学校组织的春游, 出发前牛牛准备往背包里装入一些零食, 牛牛的背包容量为w。
// 牛牛家里一共有n袋零食, 第i袋零食体积为v[i]。
// 牛牛想知道在总体积不超过背包容量的情况下,他一共有多少种零食放法(总体积为0也算一种放法)。
// 输入描述：
// 输入包括两行
// 第一行为两个正整数n和w(1 <= n <= 30, 1 <= w <= 2 * 10^9),表示零食的数量和背包的容量
// 第二行n个正整数v[i](0 <= v[i] <= 10^9),表示每袋零食的体积
// 输出描述：
// 输出一个正整数, 表示牛牛一共有多少种零食放法。
// 测试链接 : https://www.luogu.com.cn/problem/P4799
class SnacksWaysBuyTickets
{
 private:
  static const int MAXN = 40;
  static const int MAXM = 1 << 20;

  vector<long> lsum;

  vector<long> rsum;

  SnacksWaysBuyTickets()
  {
    lsum.resize(MAXM);
    rsum.resize(MAXM);
  }

 public:
  // 直接展开会超时，这个时候就需要双向展开
  // 双向广搜
  int snacksWaysBuyTickets(int n, int w, vector<int>& v)
  {
    // 左边
    int lsize = f(0, n >> 1, 0, w, lsum, v, 0);
    // 右边
    int rsize = f(n >> 1, n, 0, w, rsum, v, 0);
    std::sort(lsum.begin(), lsum.begin() + lsize);
    std::sort(rsum.begin(), rsum.begin() + rsize);
    long ans = 0;
    // 滑动窗口
    for (int i = lsize - 1, j = 0; i >= 0; i--)
    {
      while (j < rsize && lsum[i] + rsum[j] <= w)
      {
        j++;
      }
      ans += j;
    }
    return ans;
  }

  // arr[i....e]结束，e再往右不展开了！
  // s 包内零食体积累加和，
  // w 背包容量
  // arr[j] 需要填的结果在数组arr的 i 位置上
  // 返回值 : arr数组填到了什么位置！
  int f(int i, int e, long s, long w, vector<long>& arr, vector<int>& v, int j)
  {
    if (s > w)
    {
      // 零食装不下了
      return j;
    }
    // s <= w
    if (i == e)
    {
      // 所有的零食都尝试完了，填上最终可以得到的零食体积累加和
      arr[j++] = s;
    }
    else
    {
      // 不要 arr[i] 位置的数
      j = f(i + 1, e, s, w, arr, v, j);
      // 要 arr[i]位置的数
      j = f(i + 1, e, s + v[i], w, arr, v, j);
      // 要和不要都会把结果最终填入arr，因此需要得到这两轮后的 j
    }
    return j;
  }
};
