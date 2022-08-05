#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Solution
{
 public:
  long lowBit(long x) { return x & -x; }

  bool canPermutePalindrome(string s)
  {
    long n1 = 0;
    long n2 = 0;
    long n3 = 0;
    long n4 = 0;
    for (int i = 0; i < s.length(); i++)
    {
      if (s[i] > 191)
      {
        n4 ^= ((long) 1 << (s[i] - 192));
      }
      else if (s[i] > 127)
      {
        n3 ^= ((long) 1 << (s[i] - 128));
      }
      else if (s[i] > 63)
      {
        n2 ^= ((long) 1 << (s[i] - 64));
      }
      else
      {
        n1 ^= ((long) 1 << s[i]);
      }
    }
    if (s.length() & 1)
    {
      long a1 = (n1 - lowBit(n1)) | n2 | n3 | n4;
      long a2 = (n2 - lowBit(n2)) | n1 | n3 | n4;
      long a3 = (n3 - lowBit(n3)) | n1 | n2 | n4;
      long a4 = (n4 - lowBit(n4)) | n1 | n2 | n3;
      // cout << n1 << " " << n2 << " " << n3 << " " << n4 << endl;
      // cout << a1 << " " << a2 << " " << a3 << " " << a4 << endl;
      return (!!!a1 || !!!a2 || !!!a3 || !!!a4);
    }
    else
    {
      return !!!(n1 | n2 | n3 | n4);
    }
  }
};

int main()
{
  string str = "aab";
  Solution s;
  cout << s.canPermutePalindrome(str) << endl;
  return 0;
}
