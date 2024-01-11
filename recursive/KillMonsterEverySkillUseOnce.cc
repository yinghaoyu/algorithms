#include <cstdint>
#include <vector>

using namespace std;

// 现在有一个打怪类型的游戏，这个游戏是这样的，你有n个技能
// 每一个技能会有一个伤害，
// 同时若怪物小于等于一定的血量，则该技能可能造成双倍伤害
// 每一个技能最多只能释放一次，已知怪物有m点血量
// 现在想问你最少用几个技能能消灭掉他(血量小于等于0)
// 技能的数量是n，怪物的血量是m
// i号技能的伤害是kill[i]，i号技能触发双倍伤害的血量最小值是blood[i]
// 1 <= n <= 10
// 1 <= m、kill[i]、blood[i] <= 10^6
class KillMonsterEverySkillUseOnce
{
 public:
  int minCost(vector<int> kill, vector<int> blood, int n, int rest) { return f(kill, blood, n, 0, rest); }

  // n : 一共几个技能
  // i : 当前来到了第几号技能
  // r : 怪兽目前的剩余血量
  int f(vector<int>& kill, vector<int>& blood, int n, int i, int r)
  {
    if (r <= 0)
    {
      // 之前的决策已经让怪兽挂了！返回使用了多少个技能
      return i;
    }
    // r > 0
    if (i == n)
    {
      // 无效，之前的决策无效
      return INT32_MAX;
    }
    // 返回至少需要几个技能可以将怪兽杀死
    int ans = INT32_MAX;
    for (int j = i; j < n; j++)
    {
      swap(kill, blood, i, j);
      ans = std::min(ans, f(kill, blood, n, i + 1, r - (r > blood[i] ? kill[i] : 2 * kill[i])));
      swap(kill, blood, i, j);
    }
    return ans;
  }

  void swap(vector<int>& kill, vector<int>& blood, int i, int j)
  {
    std::swap(kill[i], kill[j]);
    std::swap(blood[i], blood[j]);
  }
};
