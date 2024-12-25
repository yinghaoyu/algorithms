#include <algorithm>
#include <vector>

// 普里姆最小生成树
// 算法思想：
// （1）可以从任意节点出发来寻找最小生成树
// （2）某个点加入到被选取的点中后，解锁这个点出发的所有新边
// （3）在所有解锁的边中选最小的边，然后看这个边加入到被解锁的点中后会不会形成环
// （4）如果会，舍弃当前边，返回第3步；如果不会，保留当前边，将该边的指向点解锁，此边加入到结果中，返回第2步
// （5）当所有点都被解锁时，最小生成树就得到了

// @sa https://www.bilibili.com/video/BV1sK4y1F7LH/
using namespace std;

class PrimStatic
{
 private:
  const static int MAXN = 5001;
  const static int MAXM = 400001;
  int n, m;

  // 链式前向星建图
  vector<int> head = vector<int>(MAXN);

  vector<int> next = vector<int>(MAXM);

  vector<int> to = vector<int>(MAXM);

  vector<int> weight = vector<int>(MAXM);

  int cnt;

  // 改写的堆结构
  vector<vector<int>> heap = vector<vector<int>>(MAXN, vector<int>(2));

  // where[v] = -1，表示v这个节点，从来没有进入过堆
  // where[v] = -2，表示v这个节点，已经弹出过了
  // where[v] = i(>=0)，表示v这个节点，在堆上的i位置
  vector<int> where = vector<int>(MAXN);

  // 堆的大小
  int heapSize;

  // 找到的节点个数
  int nodeCnt;

  void build()
  {
    cnt = 1;
    heapSize = 0;
    nodeCnt = 0;
    std::fill(heap.begin() + 1, heap.begin() + n + 1, 0);
    std::fill(where.begin() + 1, where.begin() + n + 1, -1);
  }

  void addEdge(int u, int v, int w)
  {
    next[cnt] = head[u];
    to[cnt] = v;
    weight[cnt] = w;
    head[u] = cnt++;
  }

  // 当前处理的是编号为ei的边！
  void addOrUpdateOrIgnore(int ei)
  {
    int v = to[ei];
    int w = weight[ei];
    // 去往v点，权重w
    if (where[v] == -1)
    {
      // v这个点，从来没有进入过堆！
      heap[heapSize][0] = v;
      heap[heapSize][1] = w;
      where[v] = heapSize++;
      heapInsert(where[v]);
    }
    else if (where[v] >= 0)
    {
      // v这个点的记录，在堆上的位置是where[v]
      heap[where[v]][1] = std::min(heap[where[v]][1], w);
      heapInsert(where[v]);
    }
  }

  void heapInsert(int i)
  {
    while (heap[i][1] < heap[(i - 1) / 2][1])
    {
      swap(i, (i - 1) / 2);
      i = (i - 1) / 2;
    }
  }

  int u;

  int w;

  // 堆顶的记录：节点 -> u、到节点的花费 -> w
  void pop()
  {
    u = heap[0][0];
    w = heap[0][1];
    swap(0, --heapSize);
    heapify(0);
    where[u] = -2;
    nodeCnt++;
  }

  void heapify(int i)
  {
    int l = i * 2 + 1;
    while (l < heapSize)
    {
      int best = l + 1 < heapSize && heap[l + 1][1] < heap[l][1] ? l + 1 : l;
      best = heap[best][1] < heap[i][1] ? best : i;
      if (best == i)
      {
        break;
      }
      swap(best, i);
      i = best;
      l = i * 2 + 1;
    }
  }

  bool isEmpty() { return heapSize == 0; }

  // 堆上，i位置的信息 和 j位置的信息 交换！
  void swap(int i, int j)
  {
    int a = heap[i][0];
    int b = heap[j][0];
    where[a] = j;
    where[b] = i;
    vector<int> tmp = std::move(heap[i]);
    heap[i] = heap[j];
    heap[j] = tmp;
  }

  int prim()
  {
    // 1节点出发
    nodeCnt = 1;
    where[1] = -2;
    for (int ei = head[1]; ei > 0; ei = next[ei])
    {
      addOrUpdateOrIgnore(ei);
    }
    int ans = 0;
    while (!isEmpty())
    {
      pop();
      ans += w;
      for (int ei = head[u]; ei > 0; ei = next[ei])
      {
        addOrUpdateOrIgnore(ei);
      }
    }
    return ans;
  }
};
