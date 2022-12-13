#include <iostream>
#include <cstdio>
#include <unordered_set>
#include <cmath>
#include <iomanip>
#include <vector>

using namespace std;

struct hashFunction
{
  size_t operator()(const pair<int, int> &x) const
  {
    return x.first ^ x.second;
  }
};

void move_head(char c, int d, pair<int,int> &head) {
    switch(c) {
        case 'U':
            head.first += d;
            break;
        case 'D':
            head.first -= d;
            break;
        case 'R':
            head.second += d;
            break;
        case 'L':
            head.second -= d;
            break;
    }
}

int distance(const pair<int, int> &head, const pair<int, int> &tail) {
    return pow(head.first - tail.first, 2) + pow(head.second - tail.second, 2);
}

void move_tail(const pair<int, int> &head, pair<int, int> &tail, unordered_set<pair<int,int>, hashFunction> &visited) {
    if (abs(head.first - tail.first) == 2) {
        if (abs(head.second - tail.second) >= 1) {
            tail.second += (head.second - tail.second) / abs(head.second - tail.second);
        }
        tail.first += (head.first - tail.first) / abs(head.first - tail.first);
    } else if (abs(head.second - tail.second) == 2) {
        if (abs(head.first - tail.first) >= 1) {
            tail.first += (head.first - tail.first) / abs(head.first - tail.first);
        }
        tail.second += (head.second - tail.second) / abs(head.second - tail.second);
    }
}

int main()
{
    string s;
    unordered_set<pair<int,int>, hashFunction>visited;
    vector<pair<int,int>> tail;
    const int MAX = 10;
    tail.resize(MAX);
    visited.insert(tail.at(0));
    while (getline(cin, s)) {
        cout << s << "\n";
        char c;
        int d;
        sscanf(s.c_str(), "%c %d", &c, &d);
        for (int i = 0; i < d; i++) {
            move_head(c, 1, tail.at(0));
            for (int j = 0; j < MAX - 1; j++) {
                move_tail(tail.at(j), tail.at(j + 1), visited);
            }
            // for (int j = 0; j < MAX; j++) {
            //     cout << "*** tail(" << j << ") -> (" << setw(3) << tail.at(j).first << ", " << setw(3) << tail.at(j).second << ")" << endl;
            // }
            visited.insert(tail.at(MAX - 1));
        }
    }
    cout << visited.size() << "\n";
    return 0;
}
