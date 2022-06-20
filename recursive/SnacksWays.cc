#include <iostream>
#include <map>
#include <vector>

using namespace std;

// 题目描述：
// 背包容量为w，第i袋零食体积为arr[i]，不超过背包容量下，一共有多少种放法？
// 每件零食可选择放或者不放，总体积为0也算一种放法
//
// 思路：分治法
//
// 测试链接：https://www.nowcoder.com/questionTerminal/d94bb2fa461d42bcb4c0f2b94f5d4281

class SnacksWays
{
 public:
  static long ways(vector<int> &arr, int bag)
  {
    if (arr.size() == 0)
    {
      return 0;
    }
    if (arr.size() == 1)
    {
      return arr[0] <= bag ? 2 : 1;
    }
    // 分治法
    int mid = (arr.size() - 1) >> 1;
    map<long, long> lmap;
    // 在0 ~ mid之间的方法数
    long ways = process(arr, 0, mid, 0, bag, lmap);
    map<long, long> rmap;
    // 在mid+1 ~ arr.size()-1之间的方法数
    ways += process(arr, mid + 1, arr.size() - 1, 0, bag, rmap);
    // 生成右边前缀和map
    map<long, long> rpre;
    long pre = 0;
    for (auto entry : rmap)
    {
      pre += entry.second;
      rpre[entry.first] = pre;
    }
    // 左边的map去匹配
    for (auto entry : lmap)
    {
      long lweight = entry.first;
      long lways = entry.second;
      // 从右边到小于等于该数且离他最近的那个数
      auto floor = rpre.lower_bound(bag - lweight);
      // 为什么要计算前缀和？
      // 右侧只要零食大小不超过floor，都视为符合条件，找出他们的方法数
      if (floor != rpre.end())
      {
        long rways = rpre.at(floor->first);
        ways += lways * rways;  // 左右两边组合的方法数
      }
    }
    // 为什么要加1，左边全不选，右边也全不选
    return ways + 1;
  }

  // arr 30
  // process(arr, 0, 14, 0, bag, map)

  // process(arr, 15, 29, 0, bag, map)

  // 从index出发，到end结束
  // 之前的选择，已经形成的累加和sum
  // 零食[index....end]自由选择，出来的所有累加和，不能超过bag，每一种累加和对应的方法数，填在map里
  // 最后不能什么货都没选
  static long process(vector<int> &arr, int index, int end, long sum, long bag, map<long, long> &map)
  {
    if (sum > bag)
    {
      return 0;
    }
    // sum <= bag
    if (index > end)
    {
      // 所有商品自由选择完了！
      if (sum != 0)
      {
        if (map.find(sum) == map.end())
        {
          map[sum] = 1L;
        }
        else
        {
          map[sum]++;
        }
        return 1;
      }
      else
      {
        return 0;
      }
    }
    // sum <= bag 并且 index <= end(还有货)
    // 1) 不要当前index位置的货
    long ways = process(arr, index + 1, end, sum, bag, map);

    // 2) 要当前index位置的货
    ways += process(arr, index + 1, end, sum + arr[index], bag, map);
    return ways;
  }
};

int main()
{
  return 0;
}
