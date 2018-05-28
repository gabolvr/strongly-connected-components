#ifndef TRANSLATOR_HPP
#define TRANSLATOR_HPP

#include <string>
#include <vector>

using namespace std;

class translator {
 public:
  vector<string>* dict;
  static string id(int);
  string translate(int i);
  translator(vector<string>* d = nullptr);
  translator(int n_v);
};

#endif  // TRANSLATOR_HPP
