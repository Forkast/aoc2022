#include <iostream>
#include <string>
#include <vector>
#include <functional>

using namespace std;

struct monkey {
  int id;
  vector<long long> items;
  function<bool(long long)> divisible;
  int t;
  int f;
  function<long long(long long)> fun;
  int inspections = 0;
};

vector<long long> split(string s) {
  string delim = ",";
  size_t pos;
  vector<long long> ret;
  do {
    pos = s.find(delim);
    int input;
    sscanf(s.substr(0, pos).c_str(), "%d", &input);
    ret.push_back(input);
    s.erase(0, pos + delim.length());
  } while (pos != string::npos);
  return ret;
}

function<long long(long long)> parse_function(string s) {
  if (s.at(0) == '+') {
    int a;
    sscanf(s.c_str(), "+ %d", &a);
    return [a] (long long old) -> long long { return old + a;};
  } else {
    int a;
    sscanf(s.c_str(), "* %d", &a);
    if (s.find("old") != string::npos) {
      return [a] (long long old) -> long long { return old * old;};
    } else {
      return [a] (long long old) -> long long { return old * a;};
    }
  }
}

int main() {
  string s;
  vector<monkey> monkeys;
  int nww = 1;
  while (getline(cin, s)) {
    monkey m;
    sscanf(s.c_str(), "Monkey %d:", &m.id);
    getline(cin, s);
    m.items = split(s.substr(string("  Starting items: ").length(), string::npos));
    getline(cin, s);
    m.fun = parse_function(s.substr(string("  Operation: new = old ").length(), string::npos));
    getline(cin, s);
    int d;
    sscanf(s.c_str(), " Test: divisible by %d:", &d);
    m.divisible = [d](long long val) -> bool { return val % d == 0; };
    getline(cin, s);
    sscanf(s.c_str(), "    If true: throw to monkey %d:", &m.t);
    getline(cin, s);
    sscanf(s.c_str(), "    If false: throw to monkey %d:", &m.f);
    nww *= d;
    getline(cin, s);
    monkeys.push_back(m);
  }
  for (int i = 0; i < 10000; i++) {
  cout << i << "\n";
    for (auto & m : monkeys) {
      for (auto item : m.items) {
        long long value = m.fun(item) % nww;
        if (m.divisible(value)) {
          monkeys.at(m.t).items.push_back(value);
        } else {
          monkeys.at(m.f).items.push_back(value);
        }
        m.inspections++;
      }
      m.items.clear();
    }
  }
  long long max[2] = {0, 0};
  for (auto m : monkeys) {
    if (m.inspections > max[0]) {
      max[1] = max[0];
      max[0] = m.inspections;
    }
    cout << m.id << " " << m.inspections << "\n";
  }
  cout << "monkey business: " << max[0] * max[1] << "\n";
}
