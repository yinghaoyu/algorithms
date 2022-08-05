#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

// 问题描述：找到数组中出现频率大于N/K次的数

class FindKMajority
{
 public:
  // 超级水王问题
  //找到数组中出现的频率大于N/2次的数
  static void printHalfMajor(vector<int> &arr)
  {
    int cand = 0;
    int HP = 0;
    for (int i = 0; i < arr.size(); i++)
    {
      if (HP == 0)  // 血量为0，选择一个候选人
      {
        cand = arr[i];
        HP = 1;
      }
      // 找两个不同的数，两两想消
      else if (arr[i] == cand)
      {
        HP++;
      }
      else
      {
        HP--;
      }
    }
    if (HP == 0)
    {
      cout << "no such number." << endl;
      return;
    }
    // 为何要再遍历1次？
    // 假设arr = {1, 2, 1, 2, 3, 3}
    // 3明显不符合
    HP = 0;
    for (int i = 0; i < arr.size(); i++)
    {
      if (arr[i] == cand)
      {
        HP++;
      }
    }
    if (HP > arr.size() / 2)
    {
      cout << cand << endl;
    }
    else
    {
      cout << "no such number." << endl;
    }
  }

  // 超级水王扩展，摩尔投票
  static void printKMajor(vector<int> &arr, int K)
  {
    if (K < 2)
    {
      cout << "the value of K is invalid." << endl;
      return;
    }
    // 攒候选，cands，候选表，最多K-1条记录！ > N / K次的数字，最多有K-1个
    // 为什么候选表要K-1个位置？
    // N/2 1个候选人
    // N/3 2个候选人
    // N/K K-1个候选人
    // 相当于把其中1份的数和其他K-1份的数相消
    unordered_map<int, int> cands;
    for (int i = 0; i != arr.size(); i++)
    {
      if (cands.find(arr[i]) != cands.end())
      {
        // 是候选，血量++
        cands[arr[i]]++;
      }
      else
      {
        // arr[i] 不是候选
        if (cands.size() == K - 1)  // 候选满了
        {
          // 当前数肯定不要！，每一个候选付出1点血量，血量变成0的候选，要删掉！
          allCandsMinusOne(cands);
        }
        else
        {
          cands[arr[i]] = 1;
        }
      }
    }
    // 所有可能的候选，都在cands表中！遍历一遍arr，每个候选收集真实次数

    unordered_map<int, int> reals = getReals(arr, cands);
    bool hasPrint = false;
    for (auto set : cands)
    {
      int key = set.first;
      if (reals[key] > arr.size() / K)
      {
        hasPrint = true;
        cout << key << " ";
      }
    }
    cout << (hasPrint ? "" : "no such number.") << endl;
  }

  static void allCandsMinusOne(unordered_map<int, int> &map)
  {
    vector<int> removeList;
    for (auto set : map)
    {
      int key = set.first;
      int value = set.second;
      if (value == 1)
      {
        removeList.push_back(key);
      }
      map[key] = value - 1;
    }
    for (int removeKey : removeList)
    {
      map.erase(removeKey);
    }
  }

  static unordered_map<int, int> getReals(vector<int> &arr, unordered_map<int, int> &cands)
  {
    unordered_map<int, int> reals;
    for (int i = 0; i != arr.size(); i++)
    {
      int curNum = arr[i];
      if (cands.find(curNum) != cands.end())
      {
        if (reals.find(curNum) != reals.end())
        {
          reals[curNum]++;
        }
        else
        {
          reals[curNum] = 1;
        }
      }
    }
    return reals;
  }

  static void test()
  {
    vector<int> arr = {1, 2, 3, 1, 1, 2, 1};
    printHalfMajor(arr);
    int K = 4;
    printKMajor(arr, K);
  }
};

int main()
{
  return 0;
}
