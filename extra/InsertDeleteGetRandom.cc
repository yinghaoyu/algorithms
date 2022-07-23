#include <iostream>
#include <random>
#include <unordered_map>
#include <vector>

using namespace std;

// 问题描述：O(1) 时间插入、删除和获取随机元素
// 测试链接： https://leetcode.com/problems/insert-delete-getrandom-o1/
class InsertDeleteGetRandom
{
 public:
  class RandomizedSet
  {
   private:
    unordered_map<int, int> keyIndexMap;

    unordered_map<int, int> indexKeyMap;

    int size;

   public:
    RandomizedSet() { size = 0; }

    bool insert(int val)
    {
      if (keyIndexMap.find(val) == keyIndexMap.end())
      {
        keyIndexMap[val] = size;
        indexKeyMap[size++] = val;
        return true;
      }
      return false;
    }

    bool remove(int val)
    {
      if (keyIndexMap.find(val) != keyIndexMap.end())
      {
        int deleteIndex = keyIndexMap[val];
        int lastIndex = --size;
        int lastKey = indexKeyMap[lastIndex];
        keyIndexMap[lastKey] = deleteIndex;
        indexKeyMap[deleteIndex] = lastKey;
        keyIndexMap.erase(val);
        indexKeyMap.erase(lastIndex);
        return true;
      }
      return false;
    }

    static int getRandom(int min, int max)
    {
      random_device seed;                            // 硬件生成随机数种子
      ranlux48 engine(seed());                       // 利用种子生成随机数引
      uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
      int res = distrib(engine);                     // 随机数
      return res;
    }

    int getRandom()
    {
      if (size == 0)
      {
        return -1;
      }
      int randomIndex = getRandom(0, size - 1);
      return indexKeyMap.at(randomIndex);
    }
  };
};

int main()
{
  return 0;
}
