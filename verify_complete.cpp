#include <iostream>
#include <map>
#include <vector>

using namespace std;
/*
We want to test if all nodes are in the scc, or if there is one node that
appears twice
*/
int main() {
  int n;
  map<int, bool> quant;
  vector<int> repeated;
  vector<int> missing;
  while (cin >> n) {
    if (quant[n] == false)
      quant[n] = true;
    else
      repeated.push_back(n);
  }
  int i = 0;
  for (map<int, bool>::iterator it = quant.begin(); it != quant.end();
       it++, i++) {
    if (i != it->first) {
      for (; i < it->first; i++) missing.push_back(i);
    }
  }
  if (!repeated.empty()) {
    cout << "repeated vertices" << endl;
    for (std::vector<int>::iterator i = repeated.begin(); i != repeated.end();
         ++i) {
      cout << *i << " ";
    }
  }
  cout << endl;
  if (!missing.empty()) {
    cout << "missing vertices" << endl;
    for (std::vector<int>::iterator i = missing.begin(); i != missing.end();
         ++i) {
      cout << *i << " ";
    }
  }
  cout << endl;
  if (missing.empty() && repeated.empty()) cout << "It seems Ok";

  return 0;
}
