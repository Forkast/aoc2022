#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>

using namespace std;

struct state {
    set<string> open_gates;
    int score = 0;
    bool available = false;
};

struct node {
    set<string> neigh;
    int rate;
};

set<string> split(string s) {
  string delim = ",";
  size_t pos;
  set<string> ret;
  do {
    pos = s.find(delim);
    char input[20];
    sscanf(s.substr(0, pos).c_str(), "%[^,]", input);
    ret.insert(input);
    s.erase(0, pos + delim.length() + 1);
  } while (pos != string::npos);
  return ret;
}

set<string> parse(const string &s) {
    if (s.starts_with(" tunnels lead to valves ")) {
        return split(s.substr(string(" tunnels lead to valves ").length(), string::npos));
    } else {
        return split(s.substr(string(" tunnel leads to valve ").length(), string::npos));
    }
}

int main() {
    string s;
    map<string, node> nodes;
    map<string, vector< vector<state>>> states;
    cout << "parse\n";
    while (getline(cin, s)) {
        char name[3];
        node n;
        sscanf(s.c_str(), "Valve %[^ ] has flow rate=%d", name, &n.rate);
        cout << s << "\n";
        n.neigh = parse(s.substr(s.find(";") + 1, string::npos));
        nodes[string(name)] = n;
        cout << "name *" << name << "* : " << n.rate << "\n";
        for (auto ne : n.neigh) {
            cout << "  neigh *" << ne << "*\n";
        }
    }
    cout << "end parse\n";
    for (const auto &[name, n] : nodes) {
        states[name].resize(31);
    }
    states["AA"].at(1).available = true;
    for (int i = 1; i < 30; i++) {
        for (auto &[sk, sv] : states) {
            if (sv[i].available) {
            // you can open this valve or traverse to nearby
                cout << "-- consider " << sk << " -- at " << i << "min " << nodes[sk].rate << " rate\n";
                cout << "   current score is " << sv[i].score << "\n";
                if (!sv[i].open_gates.contains(sk)) { // if not open already
                    cout << "  is not open - ";
                    if (/*nodes[sk].rate > 0 && */sv[i + 1].score < sv[i].score + nodes[sk].rate * (30 - i)) {
                        states[sk][i + 1].push_back(sv[i]);
                        states[sk][i + 1].open_gates.insert(sk);
                        states[sk][i + 1].score += nodes[sk].rate * (30 - i);
                        states[sk][i + 1].available = true;
                        cout << states[sk][i + 1].score << " opening\n";
                    } else if (nodes[sk].rate > 0 && sv[i + 1].score == sv[i].score + nodes[sk].rate * (30 - i)) {
                        cout << " *** equal ***\n";
                    } else {
                        cout << "but is jammed or worse\n";
                    }
                }
                cout << "go to";
                for (auto &neigh : nodes[sk].neigh) { //traverse
                    cout << " " << states[neigh][i + 1].score << neigh << sv[i].score;
                    if (states[neigh][i + 1].score < sv[i].score || !states[neigh][i + 1].available) {
                        cout << "*";
                        states[neigh][i + 1] = sv[i];
                        states[neigh][i + 1].available = true;
                    } else if (states[neigh][i + 1].score == sv[i].score) {
                        cout << "=";
                    }
                }
                cout << "\n";
                cout << "\n";
            }
        }
    }
    int max = 0;
    set<string> open;
    for (auto &[sk, sv] : states) {
        if (sv[30].score > max) {
            max = sv[30].score;
            open = sv[30].open_gates;
        }
    }
    cout << max << "\n";
    cout << "open";
    for (auto s1 : open) {
        cout << " " << s1;
    }
    cout << "\n";
}
