#include <iostream>

using namespace std;

// 判断一个数字是否是若干数量(数量>1)的连续正整数的和
class IsSumOfConsecutiveNumbers
{
 public:
  bool is1(int num)
  {
    for (int start = 1, sum; start <= num; start++)
    {
      sum = start;
      for (int j = start + 1; j <= num; j++)
      {
        if (sum + j > num)
        {
          break;
        }
        if (sum + j == num)
        {
          return true;
        }
        sum += j;
      }
    }
    return false;
  }

  // 分析：num=a+(a+1)+(a+2)+…+(a+n-1)=(2a+n-1)n/2，而2a+n-1与n肯定一个为奇数一个为偶数，
  // 即num一定要有一个奇数因子，而所有2^n 都没有奇数因子， 因此肯定不符合条件。
  // 除了2^n之外的数一定有一个奇数因子。
  // 再证明只要num有一个奇数因子，即num!=2^n，num就可以写成连续n个自然数之和。
  // 假设M有一个奇数因子a，则num=ab。
  // 1、若b也是奇数，只要b-(a-1)/2>0，M就可以写成以b-(a-1)/2开头的连续a个自然数；
  // 将这条结论里的a和b调换，仍然成立。15=3*5=1+2+3+4+5=4+5+6.
  // 2、若b是偶数，则我们有一个奇数a和一个偶数b。
  // 2.1 若b-(a-1)/2>0，M就可以写成以b-(a-1)/2开头的连续a个自然数。24=3*8=7+8+9.
  // 2.2 若(a+1)/2-b>0，M就可以写成以(a+1)/2-b开头的连续2b个自然数。38=19*2=8+9+10+11.
  // 上述两个不等式必然至少有一个成立，所以可以证明，只要M有一个奇数因子，就一定可以写成连续n个自然数之和。
  // 所以只要 num 不是 2^n，就可以满足条件
  bool is2(int num) { return (num & (num - 1)) != 0; }
};

int main()
{
  IsSumOfConsecutiveNumbers s;
  for (int num = 1; num < 200; num++)
  {
    if (s.is1(num) != s.is2(num))
    {
      cout << "Opps!" << endl;
      break;
    }
  }
}
