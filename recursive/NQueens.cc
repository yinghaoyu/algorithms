#include <iostream>
#include <algorithm>
#include <ctime>
#include <memory.h>
#include <random>

using namespace std;

class NQueens
{
  public:
    static int num1(int n)
    {
     if (n < 1)
     {
       return 0;
     }
     int* record = (int*)malloc(sizeof(int) * n);
     int ans = process1(0, record, n);
     free(record);
     return ans;
   }

   // 当前来到i行，一共是0~N-1行
   // 在i行上放皇后，所有列都尝试
   // 必须要保证跟之前所有的皇后不打架
   // int[] record record[x] = y 之前的第x行的皇后，放在了y列上
   // 返回：不关心i以上发生了什么，i.... 后续有多少合法的方法数
   static int process1(int i, int* record, int n)
   {
     if (i == n)
     {
       return 1;
     }
     int res = 0;
     // i行的皇后，放哪一列呢？j列，
     for (int j = 0; j < n; j++)
     {
       if (isValid(record, i, j))
       {
         record[i] = j;
         res += process1(i + 1, record, n);
       }
     }
     return res;
   }

   static bool isValid(int* record, int i, int j)
   {
     // 0..i-1
     for (int k = 0; k < i; k++)
     {
       if (j == record[k] || std::abs(record[k] - j) == std::abs(i - k))
       {
         return false;
       }
     }
     return true;
   }

   // 请不要超过32皇后问题
   static int num2(int n)
   {
     if (n < 1 || n > 32)
     {
       return 0;
     }
     // 如果你是13皇后问题，limit 最右13个1，其他都是0
     int limit = n == 32 ? -1 : (1 << n) - 1;
     return process2(limit, 0, 0, 0);
   }

   // 7皇后问题
   // limit : 0....0 1 1 1 1 1 1 1
   // 之前皇后的列影响：colLim
   // 之前皇后的左下对角线影响：leftDiaLim
   // 之前皇后的右下对角线影响：rightDiaLim
   static int process2(int limit, int colLim, int leftDiaLim, int rightDiaLim)
   {
     if (colLim == limit)
     {
       return 1;
     }
     // pos中所有是1的位置，是你可以去尝试皇后的位置
     int pos = limit & (~(colLim | leftDiaLim | rightDiaLim));
     int mostRightOne = 0;
     int res = 0;
     while (pos != 0)
     {
       mostRightOne = pos & (~pos + 1);  // lowBit
       pos = pos - mostRightOne;
       res += process2(limit, colLim | mostRightOne, (leftDiaLim | mostRightOne) << 1,
           ((unsigned int)(rightDiaLim | mostRightOne)) >> 1);  // 无符号右移
     }
     return res;
   }

   static void test()
   {
     int n = 15;

     clock_t start = clock();
     cout << num2(n) << endl;
     clock_t end = clock();
     cout << "cost time: " << static_cast<double>(end - start) / CLOCKS_PER_SEC << "ms" << endl;

     start = clock();
     cout << num1(n) << endl;
     end = clock();
     cout << "cost time: " << static_cast<double>(end - start) / CLOCKS_PER_SEC << "ms" << endl;

   }
  };

int main()
{
  NQueens::test();
  return 0;
}