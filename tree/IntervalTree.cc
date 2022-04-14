//线段树组
// RMQ (Range Minimum/Maximum Query) 求 区间的最值 （最大值最小值）问题以及 区间求和 的问题。
#include <iostream>
#include <vector>
using namespace std;

#define SIZE 1000

// A为原数组，C为线段树组，M为懒标记数组
int A[SIZE], C[SIZE << 2], M[SIZE << 2];

void build(int p, int cl, int cr)
{
  if(cl == cr)
  {
    return void(C[p] = A[cl]); // 叶子结点
  }
  int mid = (cl + cr) >> 1;
  build(p << 1, cl, mid);
  build(p << 1 | 1, mid + 1, cr);
  C[p] = C[p << 1] + C[p << 1 | 1]; // 父节点的值是左右孩子节点的和
}

void push_down(int p, int len)
{
  if(len <= 1)
  {
    return;
  }
  C[p << 1] += M[p] * (len - len / 2); // 此处是+=，是因为原也可能是懒标记
  M[p << 1] += M[p];
  C[p << 1 | 1] += M[p] * (len - len / 2);
  M[p << 1 | 1] += M[p];
  M[p] = 0; // 向下传递完后，要清除自己的标记
}

int query(int l, int r, int p, int cl, int cr)
{
  if(cl >= l && cr <= r) // 当前区间是目标区间的子集
  {
    return C[p];
  }
  push_down(p, cr - cl + 1); //
  int mid = (cl + cr) >> 1, ans = 0;
  if(mid >= l)
  {
    ans += query(l, r, p << 1, cl, mid);
  }
  if(mid < r)
  {
    ans += query(l ,r, p << 1 | 1, mid + 1, cr);
  }
  return ans;
}

void update(int l, int r, int d, int p, int cl, int cr)
{
  if(cl >= l && cr <= r)  // 当前的区间是目标区间的子集
  {
    C[p] += d * (cr - cl + 1);
    M[p] += d;  // 懒标记，等待下次查询时再更新子节点
    return;
  }
  push_down(p, cr - cl + 1);
  int mid = (cl + cr) >> 1;
  if(mid >= l)
  {
    update(l, r, d, p << 1, cl, mid);
  }
  if(mid < r)
  {
    update(l, r, d, p << 1 | 1, mid + 1, cr);
  }
  C[p] = C[p << 1] + C[p << 1 | 1];
}
int main()
{
  ios::sync_with_stdio(false);
  int n, m;
  cin >> n >> m;
  for (int i = 1; i <= n; ++i)
    cin >> A[i];
  build(1, 1, n);
  while (m--)
  {
    int o, l, r, d;
    cin >> o >> l >> r;
    if (o == 1)
      cin >> d, update(l, r, d, 1, 1, n);
    else
      cout << query(l, r, 1, 1, n) << '\n';
  }
  return 0;
}
