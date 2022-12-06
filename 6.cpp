#include <iostream>
#include <string>

using namespace std;

int main()
{
  const int MAX_ELEMENTS = 14;
  string s;
  getline(cin, s);
  char rec[MAX_ELEMENTS];
  for (int i = 0; i < MAX_ELEMENTS; i++) {
    rec[i] = 0;
  }
  for (int i = 0; i < s.size(); i++) {
    rec[i % MAX_ELEMENTS] = s[i];
    bool flag = true;
    for (int j = 0; j < MAX_ELEMENTS; j++) {
      for (int k = j + 1; k < MAX_ELEMENTS; k++) {
        if (rec[j] == rec[k]) {
          flag = false;
        }
      }
    }
    if (flag) {
      cout << i + 1 << endl;
      break;
    }
  }

  return 0;
}
