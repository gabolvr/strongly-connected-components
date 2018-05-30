/* This module picks an undirected graph and creates a directed one by assigning
 * one direction to each of the edges */
#include <fstream>
#include <iostream>
#include <random>
using namespace std;

int main(int argc, char const *argv[]) {
  int x, y;
  string fname = argv[1];
  const string EXTENSION = ".rgraph";
  const string SPECIFIER = "_dir";
  ifstream fin(fname + EXTENSION);
  ofstream fout(fname + SPECIFIER + EXTENSION);
  srand(time(0));
  while (fin >> x >> y) {
    if ((double(rand()) / RAND_MAX) < 0.5)
      fout << x << " " << y << endl;
    else
      fout << y << " " << x << endl;
  }
  return 0;
}
