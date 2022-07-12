#include <iostream>
#include <unordered_map>

using namespace std;

// 问题描述：
// setAll(9)表示把map中所有value设置为9
// 如何实现put、get、setAll的时间复杂度都为O(1)？

class SetAll
{
 public:
  template <typename V>
  class MyValue
  {
   public:
    V value;

    long time;  // 记录value添加的时间戳

    MyValue(V v, long t)
    {
      value = v;
      time = t;
    }
  };

  template <typename K, typename V>
  class MyHashMap
  {
   private:
    unordered_map<K, MyValue<V>> map;
    long time;       // 全局时间戳
    MyValue<V> all;  // 记录setAll的时间戳及value

   public:
    MyHashMap() { time = 0; }

    // put的时候，记录value及插入的时间戳，并将全局时间戳++
    void put(K key, V value) { map.put(key, MyValue<V>(value, time++)); }

    // 记录setAll的value及时间戳，全局时间同样++
    void setAll(V value) { all = MyValue<V>(value, time++); }

    V get(K key)
    {
      if (map.find(key) == map.end())
      {
        return nullptr;
      }
      if (map.get(key).time > all.time)  // 如果put的时间戳晚于setAll，那么返回map内的值
      {
        return map.at(key).value;
      }
      else  // put的时间戳早于setAll，直接返回setAll的value
      {
        return all.value;
      }
    }
  };
};

int main()
{
  return 0;
}
