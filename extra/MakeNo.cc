#include <iostream>
#include <vector>

using namespace std;

// 问题描述：
// 输入一个int类型的值N，构造一个长度为N的数组arr并返回
// 要求任意i < k < j，满足arr[i] + arr[j] != 2 * arr[k]

// 分析：
// arr[i] + arr[j] != 2 * arr[k]等价变换，把arr[i]、arr[k]、arr[j]减1也能达到要求
// 同理 2 * arr[i] + 1 + 2 * arr[j] + 1 != 2 * arr[k] + 1也满足
// 构造两个数组，一个是奇数序列，一个是偶数序列
// odd = {1,3,5,7,9...} 满足条件
// even = {0,2,4,6,8...} 满足条件
// 把这两个数组拼接起来也满足
// arr = { 1, 3, 5, 0, 2, 4 }
// 为什么?
// ① 如果i、j、k都在左边或者右边，显然是符合条件的
// ② 如果i在左、j在右，也是满足条件的，因为奇数+偶数 != 2 * 任意数
//
// Master 公式
// T(N) = a * T(N / b) + O(N ^ d)
// 结论：
// ① 当 logb(a) > d时，时间复杂度O(N ^ logb(a))
// ② 当 logb(a) = d时，时间复杂度O((N ^ d) * logN)
// ③ 当 logb(a) < d时，时间复杂度O(N ^ d)

class MakeNo
{
 public:
  // 生成长度为size的达标数组
  // 达标：对于任意的 i<k<j，满足   [i] + [j]  != [k] * 2
  static vector<int> makeNo(int size)
  {
    if (size == 1)
    {
      return vector<int>{1};
    }
    // size
    // 一半长达标来
    // 7   :   4
    // 8   :   4
    // [4个奇数] [3个偶]
    int halfSize = (size + 1) / 2;
    vector<int> base = makeNo(halfSize);  // Master公式第一部分T(N / 2)
    // base -> 等长奇数达标来
    // base -> 等长偶数达标来
    vector<int> ans(size);
    int index = 0;
    for (; index < halfSize; index++)  // Master公式第二部分O(N)
    {
      ans[index] = base[index] * 2 + 1;
    }
    for (int i = 0; index < size; index++, i++)
    {
      ans[index] = base[i] * 2;
    }
    // 结合Master公式，T(N) = T(N / 2) + T(N)
    // a = 1, b = 2, d = 1，符合条件③，因此时间复杂度为O(N)
    return ans;
  }

  // 检验函数
  static bool isValid(vector<int> arr)
  {
    int N = arr.size();
    for (int i = 0; i < N; i++)
    {
      for (int k = i + 1; k < N; k++)
      {
        for (int j = k + 1; j < N; j++)
        {
          if (arr[i] + arr[j] == 2 * arr[k])
          {
            return false;
          }
        }
      }
    }
    return true;
  }

  static void test()
  {
    cout << "test begin" << endl;
    for (int N = 1; N < 1000; N++)
    {
      vector<int> arr = makeNo(N);
      if (!isValid(arr))
      {
        cout << "Oops!" << endl;
      }
    }
    cout << "test end" << endl;
    cout << isValid(makeNo(1042)) << endl;
    cout << isValid(makeNo(2981)) << endl;
  }
};

int main()
{
  MakeNo::test();
  return 0;
}
