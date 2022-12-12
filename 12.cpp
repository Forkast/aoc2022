
#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <queue>
#include <utility>
#include <iomanip>

using namespace std;

struct hashFunction
{
  size_t operator()(const pair<int, int> &x) const
  {
    return x.first ^ x.second;
  }
};

int main()
{
    string s;
    vector<int> dir = {-1, 1};
    vector<vector<int>> map;
    vector<vector<int>> steps;
    unordered_set<pair<int, int>, hashFunction> visited;
    queue<pair<int, int>> Q;
    pair<int, int> start, end;
    int i = 0;
    while (getline(cin, s)) {
        map.push_back({});
        for (char c : s) {
            int x = map.size() - 1;
            if (c == 'S' || c - 'a' == 0) {
                start = pair<int, int>{x, map.at(x).size()};
                Q.push(start);
                visited.insert(start);
                map.at(x).push_back('a' - 'a');
            } else if (c == 'E') {
                end = pair<int, int>{x, map.at(x).size()};
                map.at(x).push_back('z' - 'a');
            } else {
                map.at(x).push_back(c - 'a');
            }
        }
        for (auto x : map.at(map.size() - 1)) {
            cout << setw(3) << " " << x;
        }
        cout << endl;
    }
    steps.resize(map.size());
    for (auto & step : steps) {
        step.resize(map.at(0).size());
    }
    while (!Q.empty()) {
        auto curr = Q.front();
        cout << curr.first << " : " << curr.second << endl;
        Q.pop();
        for (int i = 0; i < 2; i++) {
            int delta_x = dir[i];
            int delta_y = 0;
            for (int j = 0; j < 2; j++) {
                int x = curr.first + delta_x;
                int y = curr.second + delta_y;
                if (y >= 0 && x >= 0 && x < map.size() && y < map.at(x).size()) {
                    if (!visited.contains({x, y})
                        && map.at(x).at(y) <= map.at(curr.first).at(curr.second) + 1) {
                        visited.insert({x, y});
                        Q.push({x, y});
                        steps.at(x).at(y) = steps.at(curr.first).at(curr.second) + 1;
                    }
                    if (curr == end) {
                        goto finish;
                    }
                }
                delta_y = delta_x;
                delta_x = 0;
            }
        }
    }
    finish:
    for (int i = 0; i < map.size(); i++) {
        for (int j = 0; j < map.at(i).size(); j++) {
            if (visited.contains({i, j})) {
                cout << std::setw(4) << steps.at(i).at(j);
            } else {
                cout << std::setw(4) <<  "x";
            }
        }
        cout << endl;
    }
    cout << steps.at(end.first).at(end.second) << "\n";
}
