#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

// 编号为1~100个人玩游戏，从1开始报数，报到m的人退出，然后又从1开始报数，直到小于m
// 输出剩下的人的位置
void MySolution()
{
  vector<int> res(101);  // 0号位置不用
  for (int i = 1; i < 101; i++)
  {
    res[i] = i;  // 编号从1开始
  }
  int m = 3;
  int index = 1;
  // 这种方式有缺陷，如果保留的元素 < m - 1，比如只剩1个人，那么循环不会退出
  while (res.size() > m)  // res.size() == m 的时候结束
  {
    if (index % m == 0)
    {
      res[index] = m;  // 先标记要删除的位置
    }
    if (++index >= res.size())
    {
      index = 1;
      int n = (res.size() - 1) % m;  // -1表示忽略res[0]
      for (int i = n; i > 0; i--)
      {
        // 把不够 % m的尾部移动到头部
        res.insert(res.begin() + 1, res.back());
        res.erase(res.end() - 1);
      }
      res.erase(std::remove(res.begin(), res.end(), m), res.end());
    }
  }

  for (int i = 1; i < res.size(); i++)
  {
    if (res[i] == res.back())
    {
      cout << res[i] << endl;
    }
    else
    {
      cout << res[i] << ",";
    }
  }
}

int main()
{
  return 0;
}
