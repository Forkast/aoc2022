#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <set>
#include <map>

using namespace std;

vector<pair<int,int> > split(string &s) {
  string delim = " -> ";
  size_t pos;
  vector<pair<int,int> > ret;
  do {
    pos = s.find(delim);
    pair<int,int> input;
    sscanf(s.substr(0, pos).c_str(), "%d,%d", &input.first, &input.second);
    ret.push_back(input);
    s.erase(0, pos + delim.length());
  } while (pos != string::npos);
  return ret;
}

struct line {
  line(pair<int,int> start, pair<int,int> end)
  : start{min(start, end)}, end{max(start, end)} {}
  pair<int,int> start;
  pair<int,int> end;
  
  bool is_vertical() const {
    return start.first == end.first;
  }
  
  bool operator<(const line &r) const
  {
    if (start == r.start) {
      return end < r.end;
    }
 
    return start < r.start;
  }
  
  bool contains(pair<int,int> pt) const {
    return (pt.second == start.second && pt.first >= start.first && pt.first <= end.first)
      || (pt.first == start.first && pt.second >= start.second && pt.second <= end.second);
  }
};

ostream& operator<<(ostream& os, const line& l)
{
    if (l.is_vertical()) {
      os << "v";
    } else {
      os << "h";
    }
    os << "(" << l.start.first << ":" << l.start.second << ") -> (" << l.end.first << ":" << l.end.second << ")";
    return os;
}

int main()
{
  string s;
  set<line> lines;
  map<int, set<line>> h_buckets;
  map<int, set<line>> v_buckets;
  const int MAX = 100;
  while (getline(cin, s)) {
    vector<pair<int, int> > coords = split(s);
    for (int i = 0; i < coords.size() - 1; i++) {
      lines.insert({coords.at(i), coords.at(i + 1)});
      line l{coords.at(i), coords.at(i + 1)};
      if (l.is_vertical()) {
        v_buckets[l.start.first].insert(l);
      } else {
        h_buckets[l.start.second].insert(l);
      }
    }
  }
  
  pair<int,int> source{500, 0};
  auto curr = source;
  int max = 0;
  int sum = 0;
  for (auto it = lines.begin(); it != lines.end(); it++) {
    max = it->start.second > max ? it->start.second : max;
    max = it->end.second > max ? it->end.second : max;
  }
  max += 2;
  
  pair<int,int> next[3];
  next[0] = {0, 1};
  next[1] = {-1, 1};
  next[2] = {1, 1};
  while (true) {
//      cout << "(" << curr.first << "," << curr.second << ")\n";
    bool block[3] = {curr.second + 1 == max, curr.second + 1 == max, curr.second + 1 == max};
    for (auto it = h_buckets[curr.second + 1].begin(); it != h_buckets[curr.second + 1].end(); it++) {
      if (it->contains({curr.first, curr.second + 1})) {
        block[0] = true;
      }
      if (it->contains({curr.first - 1, curr.second + 1})) {
        block[1] = true;
      }
      if (it->contains({curr.first + 1, curr.second + 1})) {
        block[2] = true;
      }
      if (block[0] && block[1] && block[2]) {
        break;
      }
    }
    for (auto it = v_buckets[curr.first].begin(); it != v_buckets[curr.first].end(); it++) {
      if (it->contains({curr.first, curr.second + 1})) {
        block[0] = true;
      }
      if (block[0] && block[1] && block[2]) {
        break;
      }
    }
    for (auto it = v_buckets[curr.first - 1].begin(); it != v_buckets[curr.first - 1].end(); it++) {
      if (it->contains({curr.first - 1, curr.second + 1})) {
        block[1] = true;
      }
      if (block[0] && block[1] && block[2]) {
        break;
      }
    }
    for (auto it = v_buckets[curr.first + 1].begin(); it != v_buckets[curr.first + 1].end(); it++) {
      if (it->contains({curr.first + 1, curr.second + 1})) {
        block[2] = true;
      }
      if (block[0] && block[1] && block[2]) {
        break;
      }
    }
    if (block[0] && block[1] && block[2]) {
//      cout << "(" << curr.first << "," << curr.second << ")\n";
      h_buckets[curr.second].insert({curr, curr});
      sum++;
      if (curr == source) {
        break;
      }
      curr = source;
    } else {
      for (int i = 0; i < 3; i++) {
        if (!block[i]) {
          curr.first += next[i].first;
          curr.second += next[i].second;
          break;
        }
      }
    }
  }

//  for (int j = 0; j < 15; j++) {
//    for (int i = 490; i < 512; i++) {
//      bool printed = false;
//      for (auto it = h_buckets[j].begin(); it != h_buckets[j].end(); it++) {
//        if (it->contains({i, j})) {
//          cout << "#";
//          printed = true;
//          break;
//        }
//      }
//      if (!printed) {
//        for (auto it = v_buckets[i].begin(); it != v_buckets[i].end(); it++) {
//          if (it->contains({i, j})) {
//            cout << "#";
//            printed = true;
//            break;
//          }
//        }
//      }
//      if (!printed) {
//        cout << ".";
//      }
//    }
//    cout << "\n";
//  }
//  cout << "\n";
  cout << sum << "\n";

  return 0;
}
