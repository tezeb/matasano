#include <cstdio>
#include <string>
#include <vector>
#include <fstream>

#include "utils.h"
#include "xorer.h"
#include "xor_utils.h"

using namespace std;

pair<int, char> run2(const string& in) {
  auto ret = bruteXor_eng_char(unhexlify(in));
  return pair<int, char>(ret.second, ret.first[0].c);
}

pair<int, char> run1(const string& t1) {
  string t2 = unhexlify(t1);
  vector<pair<char, int>> freq = frequencer(t2);
  char eta[] = " etaoin";
  pair<int, char> ret;
  ret.first = 0;
  for(auto j = eta; *j; j++) {
    char x = freq[0].first^*j;
    string tmp = (xorer(x) << (t2)).get();
    int val = getEngScore(tmp);
    if(val > ret.first) {
      ret.first = val;
      ret.second = x;
    }
  }
  return ret;
}

void run(string fname) {
	string t1;
  ifstream in(fname);
  pair<int, char> ret;
  ret.first = 0;
  string toret;
  while(!getline(in, t1).eof()) {
    auto res = run2(t1);
    if(res.first > ret.first) {
//      printf("%02d -> %s\n", 
      ret = res,toret=t1;
    }
  }
  printf("%d\t%c\n%s\n", ret.first, ret.second, toret.c_str());
  toret = unhexlify(toret);
  printf("%s", (xorer(ret.second) << toret).get().c_str());
}

int main() {
	run("4.txt");
	return 0;
}
