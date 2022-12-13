#include <iostream>
#include <string>
#include <set>

using namespace std;

set<int> cycles = {20, 60, 100, 140, 180, 220};
const int MAX = 40;

void consider_cycle(int &cycle, int &reg, int x, int &result) {
  int pos = (cycle - 1) % 40;
  if (cycles.contains(cycle)) {
    result += reg * cycle;
  }
  if (pos >= reg - 1 && pos <= reg + 1) {
    cout << "#";
  } else {
    cout << ".";
  }
  if (pos + 1 == 40) {
    cout << "\n";
  }
  reg += x;
  cycle++;
}

int main()
{
  string s;
  int cycle = 1;
  int x = 1;
  int result = 0;
  while (getline(cin, s)) {
    consider_cycle(cycle, x, 0, result);
    if (s.at(0) == 'n') {
    } else {
      int d;
      sscanf(s.c_str(), "addx %d", &d);
      consider_cycle(cycle, x, d, result);
    }
  }
  cout << result << "\n";
  return 0;
}
