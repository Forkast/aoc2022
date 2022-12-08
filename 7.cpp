
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>

using namespace std;

const string path(deque<string> dir_stack, string filename = "") {
  string s;
  while (!dir_stack.empty()) {
    s += dir_stack.front();
    dir_stack.pop_front();
  }
  return s + filename;
}

int main()
{
  string s;
  vector<string>input;
  while (getline(cin, s)) {
    input.push_back(s);
  }

  deque<string> dir_stack;
  unordered_map<string,vector<string>> directories; // files in dirs
  unordered_set<string> dirs; // all dirs
  unordered_map<string,long long> sizes; // file sizes

  for (int i = 0; i < input.size();) {
    cout << "line: " << input[i] << endl;
    string dir_path;
    if (!dir_stack.empty()) {
      cout << "top: " << dir_stack.back() << endl;
      dir_path = path(dir_stack);
    }
    if (input[i].substr(0,4) == "$ cd") {
      string dir = input[i].substr(5);
      if (dir.substr(0,2) == "..") {
        long long sum = 0;
        for (string &dir : directories[dir_path]) {
          cout << path(dir_stack, dir) << " with size " << sizes[path(dir_stack, dir)] << "\n";
          sum += sizes[path(dir_stack, dir)];
        }
        sizes[dir_path] = sum;
        cout << "summing: " << dir_path << " " << sum << "\n";
        dir_stack.pop_back();
      } else {
        dir_stack.push_back(dir);
      }
      i++;
    } else if (input[i].substr(0,4) == "$ ls") {
      i++;
      while (i < input.size() && input[i][0] != '$') {
        char filename[30];
        if (input[i][0] >= '0' && input[i][0] <= '9') {
          long long size;
          sscanf(input[i].c_str(), "%Ld %s", &size, filename);
          cout << "file " << path(dir_stack, string(filename)) << " of " << size << "\n";
          sizes[path(dir_stack, string(filename))] = size;
        } else {
          sscanf(input[i].c_str(), "dir %s", filename);
          cout << "dir " << string(filename) << "\n";
          dirs.insert(path(dir_stack, string(filename)));
        }
        directories[dir_path].push_back(string(filename));
        i++;
      }
    }
  }
  while (!dir_stack.empty()) {
    long long sum = 0;
    for (string &dir : directories[path(dir_stack)]) {
      sum += sizes[path(dir_stack, dir)];
    }
    sizes[path(dir_stack)] = sum;
    cout << "summing: " << path(dir_stack) << " " << sum << "\n";
    dir_stack.pop_back();
  }
  cout << "\n";
  long long sum = 0;
  for (const auto & [dir, size] : sizes) {
    if (size <= 100000 && dirs.find(dir) != dirs.end()) {
      sum += size;
      cout << dir << " " << size << "\n";
    } else {
      // cout << "NOT " << dir << " " << size << "\n";
    }
  }
  cout << sum << "\n";
  long long required = sizes["/"] - (70000000 - 30000000);
  int min = 70000000;
  for (const auto & dir : dirs) {
    if (sizes[dir] > required) {
        min = min < sizes[dir] ? min : sizes[dir];
    }
  }
  cout << min << "\n";
  return 0;
}
