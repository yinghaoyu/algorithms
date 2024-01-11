// 求最大公约数、最小公倍数
class GcdAndLcm
{
 public:
  // 证明辗转相除法就是证明如下关系：
  // gcd(a, b) = gcd(b, a % b)
  // 假设a % b = r，即需要证明的关系为：gcd(a, b) = gcd(b, r)
  // 证明过程：
  // 因为a % b = r，所以如下两个等式必然成立
  // 1) a = b * q + r，q为0、1、2、3....中的一个整数
  // 2) r = a − b * q，q为0、1、2、3....中的一个整数
  // 假设u是a和b的公因子，则有: a = s * u, b = t * u
  // 把a和b带入2)得到，r = s * u - t * u * q = (s - t * q) * u
  // 这说明 : u如果是a和b的公因子，那么u也是r的因子
  // 假设v是b和r的公因子，则有: b = x * v, r = y * v
  // 把b和r带入1)得到，a = x * v * q + y * v = (x * q + y) * v
  // 这说明 : v如果是b和r的公因子，那么v也是a的公因子
  // 综上，a和b的每一个公因子 也是 b和r的一个公因子，反之亦然
  // 所以，a和b的全体公因子集合 = b和r的全体公因子集合
  // 即gcd(a, b) = gcd(b, r)
  // 证明结束
  long gcd(long a, long b) { return b == 0 ? a : gcd(b, a % b); }

  // 如果是两个数a, b, 最大公约数 为c, 只要将a, b两数相乘，再除以c即可
  // 因为两数相乘时，公约数部分乘了两次，所以要除以公约数一次
  // 所以两个数a, b, 最大公约数为c, 最小公倍数 是ab / c
  long lcm(long a, long b) { return (long) a / gcd(a, b) * b; }
};
