#include <iostream>
#include <random>
#include <unordered_set>
#include <vector>

using namespace std;

class CountOfRangeSum
{
 public:
  // 通过改写归并排序，同sort/CountOfRangeSum.cc
  static int countRangeSum1(vector<int> &nums, int lower, int upper)
  {
    int n = nums.size();
    vector<long> sums(n + 1);
    for (int i = 0; i < n; ++i)
    {
      // 前缀和
      sums[i + 1] = sums[i] + nums[i];
    }
    return countWhileMergeSort(sums, 0, n + 1, lower, upper);
  }

  static int countWhileMergeSort(vector<long> &sums, int start, int end, int lower, int upper)
  {
    if (end - start <= 1)
    {
      return 0;
    }
    int mid = (start + end) / 2;
    int count = countWhileMergeSort(sums, start, mid, lower, upper) + countWhileMergeSort(sums, mid, end, lower, upper);
    int j = mid, k = mid, t = mid;
    vector<long> cache(end - start);
    for (int i = start, r = 0; i < mid; ++i, ++r)
    {
      while (k < end && sums[k] - sums[i] < lower)
      {
        k++;
      }
      while (j < end && sums[j] - sums[i] <= upper)
      {
        j++;
      }
      while (t < end && sums[t] < sums[i])
      {
        cache[r++] = sums[t++];
      }
      cache[r] = sums[i];
      count += j - k;
    }
    std::copy(cache.begin(), cache.begin() + t - start, sums.begin() + start);
    // 从cache数组为0的位置开始，复制到数组sums为start的位置，复制长度为t - start
    // System.arraycopy(cache, 0, sums, start, t - start);
    return count;
  }

  class SBTNode
  {
   public:
    long key;
    SBTNode *l;
    SBTNode *r;
    long size;  // 不同key的size
    long all;   // 总的size

    SBTNode(long k)
    {
      key = k;
      l = nullptr;
      r = nullptr;
      size = 1;
      all = 1;
    }
  };

  class SizeBalancedTreeSet
  {
   public:
    SBTNode *root;
    unordered_set<long> set;

    SizeBalancedTreeSet() { root = nullptr; }

    SBTNode *rightRotate(SBTNode *cur)
    {
      long same = cur->all - (cur->l != nullptr ? cur->l->all : 0) - (cur->r != nullptr ? cur->r->all : 0);
      SBTNode *leftNode = cur->l;
      cur->l = leftNode->r;
      leftNode->r = cur;
      leftNode->size = cur->size;
      cur->size = (cur->l != nullptr ? cur->l->size : 0) + (cur->r != nullptr ? cur->r->size : 0) + 1;
      // all modify
      leftNode->all = cur->all;
      cur->all = (cur->l != nullptr ? cur->l->all : 0) + (cur->r != nullptr ? cur->r->all : 0) + same;
      return leftNode;
    }

    SBTNode *leftRotate(SBTNode *cur)
    {
      long same = cur->all - (cur->l != nullptr ? cur->l->all : 0) - (cur->r != nullptr ? cur->r->all : 0);
      SBTNode *rightNode = cur->r;
      cur->r = rightNode->l;
      rightNode->l = cur;
      rightNode->size = cur->size;
      cur->size = (cur->l != nullptr ? cur->l->size : 0) + (cur->r != nullptr ? cur->r->size : 0) + 1;
      // all modify
      rightNode->all = cur->all;
      cur->all = (cur->l != nullptr ? cur->l->all : 0) + (cur->r != nullptr ? cur->r->all : 0) + same;
      return rightNode;
    }

    SBTNode *maintain(SBTNode *cur)
    {
      if (cur == nullptr)
      {
        return nullptr;
      }
      long leftSize = cur->l != nullptr ? cur->l->size : 0;
      long leftLeftSize = cur->l != nullptr && cur->l->l != nullptr ? cur->l->l->size : 0;
      long leftRightSize = cur->l != nullptr && cur->l->r != nullptr ? cur->l->r->size : 0;
      long rightSize = cur->r != nullptr ? cur->r->size : 0;
      long rightLeftSize = cur->r != nullptr && cur->r->l != nullptr ? cur->r->l->size : 0;
      long rightRightSize = cur->r != nullptr && cur->r->r != nullptr ? cur->r->r->size : 0;
      if (leftLeftSize > rightSize)
      {
        cur = rightRotate(cur);
        cur->r = maintain(cur->r);
        cur = maintain(cur);
      }
      else if (leftRightSize > rightSize)
      {
        cur->l = leftRotate(cur->l);
        cur = rightRotate(cur);
        cur->l = maintain(cur->l);
        cur->r = maintain(cur->r);
        cur = maintain(cur);
      }
      else if (rightRightSize > leftSize)
      {
        cur = leftRotate(cur);
        cur->l = maintain(cur->l);
        cur = maintain(cur);
      }
      else if (rightLeftSize > leftSize)
      {
        cur->r = rightRotate(cur->r);
        cur = leftRotate(cur);
        cur->l = maintain(cur->l);
        cur->r = maintain(cur->r);
        cur = maintain(cur);
      }
      return cur;
    }

    SBTNode *add(SBTNode *cur, long key, bool contains)
    {
      if (cur == nullptr)
      {
        return new SBTNode(key);
      }
      else
      {
        cur->all++;
        if (key == cur->key)
        {
          return cur;
        }
        else
        {
          // 还在左滑或者右滑
          if (!contains)
          {
            cur->size++;
          }
          if (key < cur->key)
          {
            cur->l = add(cur->l, key, contains);
          }
          else
          {
            cur->r = add(cur->r, key, contains);
          }
          return maintain(cur);
        }
      }
    }

    void add(long sum)
    {
      bool contains = set.find(sum) != set.end();
      root = add(root, sum, contains);
      set.emplace(sum);
    }

    long lessKeySize(long key)
    {
      SBTNode *cur = root;
      long ans = 0;
      while (cur != nullptr)
      {
        if (key == cur->key)
        {
          return ans + (cur->l != nullptr ? cur->l->all : 0);
        }
        else if (key < cur->key)
        {
          cur = cur->l;
        }
        else
        {
          ans += cur->all - (cur->r != nullptr ? cur->r->all : 0);
          cur = cur->r;
        }
      }
      return ans;
    }
    // > 7 8...
    // <8 ...<=7
    long moreKeySize(long key) { return root != nullptr ? (root->all - lessKeySize(key + 1)) : 0; }
  };

  static int countRangeSum2(vector<int> &nums, int lower, int upper)
  {
    // 黑盒，加入数字（前缀和），不去重，可以接受重复数字
    // < num , 有几个数？
    SizeBalancedTreeSet treeSet;
    long sum = 0;
    int ans = 0;
    treeSet.add(0);  // 一个数都没有的时候，就已经有一个前缀和累加和为0，
    for (int i = 0; i < nums.size(); i++)
    {
      sum += nums[i];
      // [sum - upper, sum - lower]
      // [10, 20] ?
      // < 10 ?  < 21 ?
      long a = treeSet.lessKeySize(sum - lower + 1);
      long b = treeSet.lessKeySize(sum - upper);
      ans += a - b;
      treeSet.add(sum);
    }
    return ans;
  }

  // for test
  static void printArray(vector<int> &arr)
  {
    for (int i = 0; i < arr.size(); i++)
    {
      cout << arr[i] << " ";
    }
    cout << endl;
  }

  // for test
  static vector<int> generateArray(int len, int varible)
  {
    vector<int> arr(len);
    for (int i = 0; i < len; i++)
    {
      arr[i] = getRandom(0, varible);
    }
    return arr;
  }

  static int getRandom(int min, int max)
  {
    random_device seed;                            // 硬件生成随机数种子
    ranlux48 engine(seed());                       // 利用种子生成随机数引
    uniform_int_distribution<> distrib(min, max);  // 设置随机数范围，并为均匀分布
    int res = distrib(engine);                     // 随机数
    return res;
  }

  static void test()
  {
    int len = 200;
    int varible = 50;
    cout << "test begin" << endl;
    for (int i = 0; i < 10000; i++)
    {
      vector<int> test = generateArray(len, varible);
      int lower = getRandom(0, varible) - getRandom(0, varible);
      int upper = lower + getRandom(0, varible);
      int ans1 = countRangeSum1(test, lower, upper);
      int ans2 = countRangeSum2(test, lower, upper);
      if (ans1 != ans2)
      {
        printArray(test);
        cout << lower << endl;
        cout << upper << endl;
        cout << ans1 << endl;
        cout << ans2 << endl;
      }
    }
    cout << "test end" << endl;
  }
};

int main()
{
  CountOfRangeSum::test();
  return 0;
}
