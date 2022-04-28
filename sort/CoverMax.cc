#include <stdbool.h>
#include <random>
#include <algorithm>
#include <queue>
#include <iostream>

using namespace std;

//给定很多线段，每个线段都有两个数[start, end]，表示线段开始位置和结束位置，左右都是闭区间。
//规定：
//1）线段的开始位置和结束位置一定都是整数
//2）两个线段重合的定义：线段重合区域的长度必须>=1才表示线段重合
//求：线段最多的重合区域中，包含了几条线段。
//
//思路：
//1）将所有线段按照开始位置从小到大排序，同时准备一个小根堆用于存放线段结束位置
//2）遍历排序后的所有线段，将此时小根堆里小于等于此时线段开始位置的数弹出，同时将此时线段结束位置的数放到小根堆里，此时小根堆的个数即是从此时线段开始位置作为重合区域左边界的重合区域所包含线段的个数。（任何重合区域的左边界必定是某个线段的左边界）

//因为此时小根堆里的数表示之前线段的右边界会穿过此时线段的左边界，而之前线段的左边界是小于此时线段的左边界的，所以小根堆的个数即是重合区域中包含线段的个数。

class CoverMax
{
  public:
    static int getRandom(int min, int max)
    {
      random_device seed;  // 硬件生成随机数种子
      ranlux48 engine(seed());  // 利用种子生成随机数引
      uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
      int res = distrib(engine);  // 随机数
      return res;
    }

    static void printLines(int** lines, int len)
    {
      for(int i = 0; i < len; i++)
      {
        cout << lines[i][0] << " " << lines[i][1] << endl;
      }
      cout << endl;
    }

    static int maxCover1(int** lines, int len)
    {
      int min = INT32_MAX;
      int max = INT32_MIN;
      for (int i = 0; i < len; i++)
      {
        min = std::min(min, lines[i][0]);
        max = std::max(max, lines[i][1]);
      }
      int cover = 0;
      for (double p = min + 0.5; p < max; p += 1)
      {
        int cur = 0;
        for (int i = 0; i < len; i++)
        {
          if (lines[i][0] < p && lines[i][1] > p)
          {
            cur++;
          }
        }
        cover = std::max(cover, cur);
      }
      return cover;
    }

    class Line
    {
      public:
        int start;
        int end;

        Line(int s, int e)
        {
          start = s;
          end = e;
        }

        bool operator<(const Line& other) const  // priority_queue此处从在要在函数尾部加const ！！！
        {
          return this->start < other.start;
        }
    };

    static int maxCover2(int** m, int len)
    {
      Line* lines = (Line*)malloc(len * sizeof(Line));
      for (int i = 0; i < len; i++)
      {
        lines[i].start  = m[i][0];
        lines[i].end = m[i][1];
      }
      std::sort(lines, lines + len);
      // 小根堆，每一条线段的结尾数值，使用默认的
      priority_queue<int, vector<int>, greater<int>> heap;
      int ans = 0;
      for (int i = 0; i < len; i++)
      {
        // lines[i] -> cur 在黑盒中，把<=cur.start 东西都弹出
        while (!heap.empty() && heap.top() <= lines[i].start)
        {
          heap.pop();
        }
        heap.emplace(lines[i].end);
        ans = std::max(ans, (int)heap.size());
      }
      return ans;
    }

    // for test
    static int** generateLines(int N, int L, int R, int* len)
    {
      *len = getRandom(1, N);
      int** ans = (int**)malloc(*len * sizeof(int*));
      for(int i = 0; i < *len; i++)
      {
        ans[i] = (int*)malloc(sizeof(int)*2);
      }
      for (int i = 0; i < *len; i++)
      {
        int a = L + getRandom(0, R - L);
        int b = L + getRandom(0, R - L);
        if (a == b)
        {
          b = a + 1;
        }
        ans[i][0] = std::min(a, b);
        ans[i][1] = std::max(a, b);
      }
      return ans;
    }
};

bool cmp(const CoverMax::Line& a, const CoverMax::Line& b)
{
  return a.start < b.start;
}

int main()
{
  CoverMax::Line l1(4, 9);
  CoverMax::Line l2(1, 4);
  CoverMax::Line l3(7, 15);
  CoverMax::Line l4(2, 4);
  CoverMax::Line l5(4, 6);
  CoverMax::Line l6(3, 7);
  // 底层堆结构，heap
  priority_queue<CoverMax::Line> heap;
  heap.emplace(l1);
  heap.emplace(l2);
  heap.emplace(l3);
  heap.emplace(l4);
  heap.emplace(l5);
  heap.emplace(l6);

  while (!heap.empty())
  {
    CoverMax::Line cur = heap.top();
    heap.pop();
    cout << cur.start << "," << cur.end << endl;
  }

  cout << "test begin" << endl;
  int N = 100;
  int L = 0;
  int R = 200;
  int testTimes = 1;
  int len = 0;
  for (int i = 0; i < testTimes; i++)
  {
    int** lines = CoverMax::generateLines(N, L, R, &len);
    //CoverMax::printLines(lines, len);
    int ans1 = CoverMax::maxCover1(lines, len);
    int ans2 = CoverMax::maxCover2(lines, len);
    if (ans1 != ans2)
    {
      cout << "Oops! " << ans1 << " " << ans2 << endl;
    }
  }
  cout << "test end" << endl;
}
