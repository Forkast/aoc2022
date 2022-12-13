#include <iostream>
#include <string>
#include <vector>
#include <memory>

using namespace std;

struct node {
    node(int value) : value(value) {}
    vector<unique_ptr<node>> nodes;
    int value;
    node(node &other) = default;
};

std::ostream& operator<<(std::ostream& os, const unique_ptr<node>& obj) {
    if (obj->value != -1) {
        os << obj->value;
    } else {
        os << "[";
        for (auto &n : obj->nodes) {
            os << n;
            if (&n != &obj->nodes.back()) {
                os << ",";
            }
        }
        os << "]";
    }
    return os;
}

void parse(const string &s, unique_ptr<node> &n);

void parse_list(const string &s, unique_ptr<node> &n) {
    // cout << "parse_list " << s << endl;
    int depth = 0;
    int value_start = 0;
    for (int i = 0; i < s.size(); i++) {
        // cout << "char " << s.at(i) << endl;
        if (s.at(i) == '[') {
            depth++;
        } else if (s.at(i) == ']') {
            depth--;
        }
        if (depth == 0) {
            if (s.at(i) == ',') {
                // cout << i << endl;
                n->nodes.push_back(make_unique<node>(-1));
                parse(s.substr(value_start, i - value_start),
                      n->nodes.at(n->nodes.size() - 1));
                value_start = i + 1;
            }
            if (i == s.size() - 1) {
                // cout << "end -> " << i << endl;
                n->nodes.push_back(make_unique<node>(-1));
                parse(s.substr(value_start, i - value_start + 1),
                      n->nodes.at(n->nodes.size() - 1));
                value_start = i + 1;
            }
        }
    }
    // cout << "end parse_list\n";
}

void parse(const string &s, unique_ptr<node> &n) {
    // cout << "parse " << s << endl;
    n = make_unique<node>(-1);
    if (s.at(0) == '[') { //list
        parse_list(s.substr(1, s.size() - 2), n);
    } else { // value
        sscanf(s.c_str(), "%d", &n->value);
    }
}

int compare(const unique_ptr<node> &t1, const unique_ptr<node> &t2) {
    if ((t1->value == -1) != (t2->value == -1)) { // convert to list if not both lists
        if (t1->value != -1) {
            t1->nodes.push_back(make_unique<node>(t1->value));
            t1->value = -1;
        }
        if (t2->value != -1) {
            t2->nodes.push_back(make_unique<node>(t2->value));
            t2->value = -1;
        }
    }
    if (t1->value == -1 && t2->value == -1) {
        for (int i = 0; i < t1->nodes.size() && i < t2->nodes.size(); i++) {
            int ret;
            if ((ret = compare(t1->nodes.at(i), t2->nodes.at(i))) != 0) {
                return ret;
            }
        }
        if (t1->nodes.size() > t2->nodes.size()) {
            return -1;
        } else if (t1->nodes.size() < t2->nodes.size()) {
            return 1;
        } else {
            return 0;
        }
    } else if (t1->value == t2->value) {
        return 0;
    } else if (t1->value < t2->value) {
        return 1;
    }
    return -1;
}


struct great {
    bool operator()(const unique_ptr<node> &lhs, const unique_ptr<node> &rhs) {
        return compare(lhs, rhs) == 1;
    }
};

int main() {
    string first;
    int i = 1;
    int sum = 0;
    vector<unique_ptr<node> > trees;
    while (getline(cin, first)) {
        string second;
        getline(cin, second);
        unique_ptr<node> t1;
        unique_ptr<node> t2;
        parse(first, t1);
        parse(second, t2);
        int ret;
        if ((ret = compare(t1, t2)) != -1) {
            sum += i;
        }
        getline(cin, first);
        i++;
        trees.push_back(move(t1));
        trees.push_back(move(t2));
    }
    sort(trees.begin(), trees.end(), great());
    for (auto &t : trees) {
        cout << t << endl;
    }
    cout << sum << "\n";
}
