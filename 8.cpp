#include <string>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int main() {
  string s;
  vector<vector<int>> trees;
  vector<vector<bool>> visible;
  vector<vector<int>> score;

  int i = 0;
  while (getline(cin, s)) {
    trees.push_back({});
    for (char c : s) {
      trees[i].push_back(c - '0');
    }
    i++;
  }
  visible.resize(trees.size(), {});
  score.resize(trees.size(), {});
  for (i = 0; i < trees.size(); i++) {
    if (i == 0 || i == trees.size() - 1) {
      visible[i].resize(trees[i].size(), true);
    } else {
      visible[i].resize(trees[i].size(), false);
    }
    visible[i][0] = true;
    visible[i][visible[i].size() - 1] = true;
    score[i].resize(trees[i].size(), 1);
  }

  for (i = 1; i < trees.size() - 1; i++) {
    int max = 0;
    int dist[10];
    fill(dist, dist + 10, 1);
    for (int j = 1; j < trees[i].size() - 1; j++) {
      if (trees[i][j] > max) {
        visible[i][j] = true;
        max = trees[i][j];
      }
      score[i][j] *= dist[trees[i][j]];
      for (int k = 0; k < 10; k++) {
        if (k <= trees[i][j]) {
          dist[k] = 1;
        } else {
          dist[k]++;
        }
      }
    }
    fill(dist, dist + 10, 1);
    max = 0;
    for (int j = trees[i].size() - 2; j > 0; j--) {
      if (trees[i][j] > max) {
        visible[i][j] = true;
        max = trees[i][j];
      }
      score[i][j] *= dist[trees[i][j]];
      for (int k = 0; k < 10; k++) {
        if (k <= trees[i][j]) {
          dist[k] = 1;
        } else {
          dist[k]++;
        }
      }
    }
  }
  for (i = 1; i < trees[0].size() - 1; i++) {
    int max = 0;
    int dist[10];
    fill(dist, dist + 10, 1);
    for (int j = 1; j < trees.size() - 1; j++) {
      if (trees[j][i] > max) {
        visible[j][i] = true;
        max = trees[j][i];
      }
      score[j][i] *= dist[trees[j][i]];
      for (int k = 0; k < 10; k++) {
        if (k <= trees[j][i]) {
          dist[k] = 1;
        } else {
          dist[k]++;
        }
      }
    }
    max = 0;
    fill(dist, dist + 10, 1);
    for (int j = trees.size() - 2; j > 0; j--) {
      if (trees[j][i] > max) {
        visible[j][i] = true;
        max = trees[j][i];
      }
      score[j][i] *= dist[trees[j][i]];
      for (int k = 0; k < 10; k++) {
        if (k <= trees[j][i]) {
          dist[k] = 1;
        } else {
          dist[k]++;
        }
      }
    }
  }
  int sum = 0;
  for (i = 0; i < trees.size(); i++) {
    for (int j = 0; j < trees[i].size(); j++) {
      if (visible[i][j]) {
        sum++;
      }
    }
  }
  cout << sum << "\n";
  int max = 0;
  for (i = 0; i < trees.size(); i++) {
    for (int j = 0; j < trees[i].size(); j++) {
      if (score[i][j] > max) {
        max = score[i][j];
      }
      cout << score[i][j] << ":";
    }
    cout << "\n";
  }
  cout << max << "\n";
}
