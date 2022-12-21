#include <iostream>
#include <string>
#include <map>
#include <set>
#include <unordered_set>
#include <vector>
#include <queue>
#include <algorithm>
#include <random>

using namespace std;

struct node {
    set<string> neigh;
    int rate;
};

struct g_node {
    vector< pair<int, string>> neigh;
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

map<string, g_node> construct_clique(const map<string, node> &nodes) {
    queue<pair<string, int>> Q;
    set<string> visited;
    int depth;
    map<string, g_node> new_nodes;
    for (const auto &[name, n] : nodes) {
        if (n.rate > 0 || name == "AA") {
            visited.clear();
            Q.push({name, 0});
            visited.insert(name);
            new_nodes[name].rate = nodes.at(name).rate;
            while (!Q.empty()) {
                auto curr = Q.front();
                Q.pop();
                for (const auto &nei : nodes.at(curr.first).neigh) {
                    if (!visited.contains(nei)) {
                        Q.push({nei, curr.second + 1});
                        visited.insert(nei);
                        if (nodes.at(nei).rate > 0) {
                            new_nodes[name].neigh.push_back({curr.second + 2, nei});
                        }
                    }
                }
            }
        }
    }
    return new_nodes;
}

void print_clique(map<string, g_node> clique) {
    cout << "clique: \n";
    for (const auto &[name, gn] : clique) {
        cout << name;
        for (const auto &[rate, nei] : gn.neigh) {
            cout << " (" << rate << ", " << nei << ", " << clique[nei].rate << ")";
        }
        cout << "\n";
    }
    cout << "end clique\n\n";
}

struct state {
    vector<string> open_gates;
    pair<string, string> current;
    int score = 0;
    bool available = false;
};

struct possible_states {
    set<state> possible;
};

pair<string,string> concat_twist(const string &s1, const string &s2) {
    if (s1.compare(s2)) {
        return {s1, s2};
    }
    return {s2, s1};
}

int solution1(const map<string, g_node> &clique) {
    const int MAX = 30;
    map<string, vector<possible_states>> dynamic_states;
    for (auto &[name, s] : clique) {
        dynamic_states[name].resize(MAX);
    }
    dynamic_states["AA"].at(0).possible.insert({{}, {{}, {}}, 0, true});
    map<string, int> max;
    for (const auto &[name, c] : clique) {
        max[name] = 0;
    }
    for (int i = 0; i < MAX; i++) {
        for (const auto &[state_name, p_states] : dynamic_states) {
            for (const auto & possible : p_states.at(i).possible) {
                if (possible.available) {
                    for (const auto &[dist, neighbour_name] : clique.at(state_name).neigh) {
                        int new_score = possible.score;
                        for (const auto & gate : possible.open_gates) {
                            new_score += clique.at(gate).rate * dist;
                        }
                        if (!possible.open_gates.contains(neighbour_name)
                          && i + dist <= MAX) {
                            state new_state = possible;
                            new_state.open_gates.insert(neighbour_name);
                            new_state.score = new_score;
                            dynamic_states.at(neighbour_name).at(i + dist).possible.insert(new_state);
                        }
                    }
                    int new_score = possible.score;
                    for (const auto & gate : possible.open_gates) {
                        new_score += clique.at(gate).rate * (MAX - i);
                    }
                    if (max.at(state_name) < new_score) {
                        max[state_name] = new_score;
                    }
                }
            }
        }
    }

    int maximum = 0;
    for (const auto &[g, m] : max) {
        maximum = maximum < m ? m : maximum;
    }
    return maximum;
}

int dfs_one(const map<string, g_node> &clique, string current, set<string> visited, int depth) {
    visited.insert(current);
    int maximum = 0;
    for (const auto &[dist, name] : clique.at(current).neigh) {
        int res = 0;
        if (!visited.contains(name) && depth - dist >= 0) {
            res = dfs_one(clique, name, visited, depth - dist);
        }
        maximum = res > maximum ? res : maximum;
    }
    return maximum + depth * clique.at(current).rate;
}

int dfs(const map<string, g_node> &clique, string current, set<string> visited, int depth) {
    visited.insert(current);
    int maximum = dfs_one(clique, "AA", visited, 26);
    for (const auto &[dist, name] : clique.at(current).neigh) {
        int res = 0;
        if (!visited.contains(name) && depth - dist >= 0) {
            res = dfs(clique, name, visited, depth - dist);
        }
        maximum = res > maximum ? res : maximum;
    }
    cout << current << " " << maximum + depth * clique.at(current).rate << "\n";
    return maximum + depth * clique.at(current).rate;
}

int solution2(map<string, g_node> &clique) {
    return dfs(clique, "AA", {}, 26);
}

int main() {
    string s;
    map<string, node> nodes;
    while (getline(cin, s)) {
        char name[3];
        node n;
        sscanf(s.c_str(), "Valve %[^ ] has flow rate=%d", name, &n.rate);
        n.neigh = parse(s.substr(s.find(";") + 1, string::npos));
        nodes[string(name)] = n;
    }
    map<string, g_node> clique = construct_clique(nodes);
    print_clique(clique);

//    cout << "end result: " << solution1(clique) << "\n";
    cout << "end result:\n" << solution2(clique) << "\n";
}
