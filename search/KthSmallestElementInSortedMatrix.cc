#include <iostream>
#include <queue>
#include <vector>

using namespace std;
using TdArray = vector<vector<int>>;

// 问题描述：
// 给你一个 n x n 矩阵 matrix ，其中每行和每列元素均按升序排序，找到矩阵中第 k 小的元素。
// 请注意，它是 排序后 的第 k 小元素，而不是第 k 个 不同 的元素。
// 你必须找到一个内存复杂度优于 O(n2) 的解决方案。

// 示例：
// 输入：matrix = [[1,5,9],[10,11,13],[12,13,15]], k = 8
// 输出：13
// 解释：矩阵中的元素为 [1,5,9,10,11,12,13,13,15]，第 8 小元素是 13

// 本题测试链接 : https://leetcode.com/problems/kth-smallest-element-in-a-sorted-matrix/

class KthSmallestElementInSortedMatrix
{
  // 堆的方法
 public:
  static int kthSmallest1(TdArray matrix, int k)
  {
    int N = matrix.size();
    int M = matrix[0].size();
    priority_queue<Node *> heap;
    vector<vector<bool>> set(N, vector<bool>(M));
    heap.push(new Node(matrix[0][0], 0, 0));
    set[0][0] = true;
    int count = 0;
    Node *ans = nullptr;
    while (!heap.empty())
    {
      // 为什么不需要所有数全进堆再pop？
      // 因为二维本组本来就是按行、按列升序的，
      // 只需要从左往右，从上往下依次进堆，pop出来的一定是最小的元素
      ans = heap.top();
      heap.pop();
      if (++count == k)
      {
        break;
      }
      int row = ans->row;
      int col = ans->col;
      if (row + 1 < N && !set[row + 1][col])
      {
        heap.push(new Node(matrix[row + 1][col], row + 1, col));
        set[row + 1][col] = true;
      }
      if (col + 1 < M && !set[row][col + 1])
      {
        heap.push(new Node(matrix[row][col + 1], row, col + 1));
        set[row][col + 1] = true;
      }
    }
    return ans->value;
  }

  class Node
  {
   public:
    int value;
    int row;
    int col;

    Node(int v, int r, int c)
    {
      value = v;
      row = r;
      col = c;
    }
  };

  class NodeComparator
  {
   public:
    static bool compare(Node o1, Node o2) { return o1.value - o2.value; }
  };

  // 二分的方法
  static int kthSmallest2(TdArray &matrix, int k)
  {
    int N = matrix.size();
    int M = matrix[0].size();
    int left = matrix[0][0];
    int right = matrix[N - 1][M - 1];
    int ans = 0;
    while (left <= right)
    {
      int mid = left + ((right - left) >> 1);
      // <=mid 有几个 <= mid 在矩阵中真实出现的数，谁最接近mid
      Info info = noMoreNum(matrix, mid);
      if (info.num < k)
      {
        left = mid + 1;
      }
      else
      {
        ans = info.near;
        right = mid - 1;
      }
    }
    return ans;
  }

  class Info
  {
   public:
    int near;

    int num;

    Info(int n1, int n2)
    {
      near = n1;
      num = n2;
    }
  };

  static Info noMoreNum(TdArray &matrix, int value)
  {
    int near = INT32_MIN;
    int num = 0;
    int N = matrix.size();
    int M = matrix[0].size();
    int row = 0;
    int col = M - 1;
    while (row < N && col >= 0)
    {
      if (matrix[row][col] <= value)
      {
        near = std::max(near, matrix[row][col]);  // near为最接近value的数
        num += col + 1;                           // num 为符合条件的数的数量
        row++;
      }
      else
      {
        col--;
      }
    }
    return Info(near, num);
  }
};

int main()
{
  TdArray matrix{
      {0, 1, 2, 3, 4, 5, 6},                // 0
      {10, 12, 13, 15, 16, 17, 18},         // 1
      {23, 24, 25, 26, 27, 28, 29},         // 2
      {44, 45, 46, 47, 48, 49, 50},         // 3
      {65, 66, 67, 68, 69, 70, 71},         // 4
      {96, 97, 98, 99, 100, 111, 122},      // 5
      {166, 176, 186, 187, 190, 195, 200},  // 6
      {233, 243, 321, 341, 356, 370, 380}   // 7
  };
  int K = 35;
  cout << KthSmallestElementInSortedMatrix::kthSmallest1(matrix, K) << endl;
  return 0;
}
