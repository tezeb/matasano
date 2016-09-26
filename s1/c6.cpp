#include <cstdio>
#include "utils.h"
#include <string>
#include "xorer.h"
#include <fstream>

using namespace std;

void run(string& t1, string& t2, int exp) {	
  int r = editDist(t1, t2);
  printf("%s:\t%d == %d\n", r==exp?"MATCH":"FAILED", r, exp);
}

string readFile(string filename) {
  ifstream file(filename);
  string ret;
  string tmp;
  while(!getline(file, tmp).eof()) {
    ret.append(tmp);
  }
  file.close();
  return ret;
}

int main() {
	string t1="this is a test";
	string t2="wokka wokka!!!";
	run(t1, t2, 37);
  string f = readFile("6.txt");
	return 0;
}
