#include "translator.hpp"

using namespace std;

static string id(int i) { return to_string(i); }
string translator::translate(int i) { return (*dict)[i]; }
translator::translator(vector<string>* d) { dict = d; }
translator::translator(int n_v) {
  dict = new vector<string>(n_v);
  for (unsigned i = 0; i < n_v; ++i) {
    (*dict)[i] = to_string(i);
  }
}
