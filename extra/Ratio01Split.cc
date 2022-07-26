#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

// 把一个01字符串切成多个部分，要求每一部分的0和1比例一样，同时要求尽可能多的划分
// 比如 : 01010101
// 01 01 01 01 这是一种切法，0和1比例为 1 : 1
// 0101 0101 也是一种切法，0和1比例为 1 : 1
// 两种切法都符合要求，但是那么尽可能多的划分为第一种切法，部分数为4
// 比如 : 00001111
// 只有一种切法就是00001111整体作为一块，那么尽可能多的划分，部分数为1
// 给定一个01字符串str，假设长度为N，要求返回一个长度为N的数组ans
// 其中ans[i] = str[0...i]这个前缀串，要求每一部分的0和1比例一样，同时要求尽可能多的划分下，部分数是多少
// 输入: str = "010100001"
// 输出: ans = [1, 1, 1, 2, 1, 2, 1, 1, 3]
class Ratio01Split
{
  // 001010010100...
 public:
  static vector<int> split(vector<int> &arr)
  {
    // key : 分子
    // value : 属于key的分母表, 每一个分母，及其 分子/分母 这个比例，多少个前缀拥有
    unordered_map<int, unordered_map<int, int>> pre;
    int n = arr.size();
    vector<int> ans(n);
    int zero = 0;  // 0出现的次数
    int one = 0;   // 1出现的次数
    for (int i = 0; i < n; i++)
    {
      if (arr[i] == 0)
      {
        zero++;
      }
      else
      {
        one++;
      }
      if (zero == 0 || one == 0)
      {
        ans[i] = i + 1;
      }
      else
      {  // 0和1，都有数量 -> 最简分数
        int g = gcd(zero, one);
        int a = zero / g;
        int b = one / g;
        // a / b 比例，之前有多少前缀拥有？ 3+1 4 5+1 6
        if (pre.find(a) == pre.end())
        {
          pre.emplace(a, unordered_map<int, int>());
        }
        if (pre.at(a).find(b) == pre.at(a).end())
        {
          pre.at(a).emplace(b, 1);
        }
        else
        {
          pre.at(a).emplace(b, pre.at(a).at(b) + 1);
        }
        ans[i] = pre.at(a).at(b);
      }
    }
    return ans;
  }

  static int gcd(int m, int n) { return n == 0 ? m : gcd(n, m % n); }

  static void main()
  {
    vector<int> arr = {0, 1, 0, 1, 0, 1, 1, 0};
    vector<int> ans = split(arr);
    for (int i = 0; i < ans.size(); i++)
    {
      cout << ans[i] << " ";
    }
  }
};

int main()
{
  return 0;
}
