#include <vector>

using namespace std;

// 一开始1~n范围上的数字都是0，一共有m个操作，每次操作为(l,r,s,e,d)
// 表示在l~r范围上依次加上首项为s、末项为e、公差为d的数列
// m个操作做完之后，统计1~n范围上所有数字的最大值和异或和
class ArithmeticSequenceDifference
{
 public:
  int getXor(vector<vector<int>>& operations, int n)
  {
    vector<int> arr(n + 2);
    for (auto& op : operations)
    {
      int l = op[0], r = op[1], s = op[2], e = op[3];
      set(arr, l, r, s, e, (e - s) / (r - l));
    }
    build(arr);
    int max = 0, ans = 0;
    for (int i = 1; i <= n; i++)
    {
      max = std::max(max, arr[i]);
      ans ^= arr[i];
    }
    return ans;
  }

  // TODO: figure it out
  void set(vector<int>& arr, int l, int r, int s, int e, int d)
  {
    arr[l] += s;
    arr[l + 1] += d - s;
    arr[r + 1] -= d + e;
    arr[r + 2] += e;
  }

  void build(vector<int>& arr)
  {
    for (int i = 1; i < arr.size(); i++)
    {
      arr[i] += arr[i - 1];
    }
    for (int i = 1; i < arr.size(); i++)
    {
      arr[i] += arr[i - 1];
    }
  }
};
