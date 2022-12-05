#include <iostream>
#include <string>
#include <cstdio>
#include <deque>
#include <vector>

using namespace std;

int main()
{
  string s;
  vector<string> input;
  string result;
  vector<deque<char>> stacks;
  stacks.resize(10);
  deque<char> temp;
  while (getline(cin, s)) {
    input.push_back(s);
  }
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < input[i].size(); j++) {
      if (input[i][j] >= 'A' && input[i][j] <= 'Z') {
        cout << j / 4 + 1 << endl;
        stacks[j / 4 + 1].push_front(input[i][j]);
      }
    }
  }
  for (int i = 10; i < input.size(); i++) {
    int num, src, dest;
    cout << input[i] << endl;
    sscanf(input[i].c_str(), "move %d from %d to %d", &num, &src, &dest);
    for (int j = 0; j < num; j++) {
      temp.push_back(stacks[src].back());
      stacks[src].pop_back();
    }
    for (int j = 0; j < num; j++) {
      stacks[dest].push_back(temp.back());
      temp.pop_back();
    }
  }
  for (int i = 1; i < 10; i++) {
    result.push_back(stacks[i].back());
  }
  cout << result;

  return 0;
}
