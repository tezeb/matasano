#include <cstdio>
#include "utils.h"
#include <string>
#include <map>
#include <algorithm>
#include <vector>
#include <fstream>

using namespace std;

namespace {
  map<char, double> freqs = {
      {'a', 0.0651738},
      {'b', 0.0124248},
      {'c', 0.0217339},
      {'d', 0.0349835},
      {'e', 0.1041442},
      {'f', 0.0197881},
      {'g', 0.0158610},
      {'h', 0.0492888},
      {'i', 0.0558094},
      {'j', 0.0009033},
      {'k', 0.0050529},
      {'l', 0.0331490},
      {'m', 0.0202124},
      {'n', 0.0564513},
      {'o', 0.0596302},
      {'p', 0.0137645},
      {'q', 0.0008606},
      {'r', 0.0497563},
      {'s', 0.0515760},
      {'t', 0.0729357},
      {'u', 0.0225134},
      {'v', 0.0082903},
      {'w', 0.0171272},
      {'x', 0.0013692},
      {'y', 0.0145984},
      {'z', 0.0007836},
      {' ', 0.1918182}
  };
	vector<pair<char,int>> frequencer(const string& a) {
    map<char, int> ret;
		for(size_t i = 0; i < a.length(); i++) {
      if(ret.find(a[i]) == ret.end())
          ret[a[i]] = 0;
      ret[a[i]]++;
		}
    vector<pair<char, int>> freq(ret.begin(), ret.end());
    sort(freq.begin(), freq.end(), 
         [](pair<char, int> p1, pair<char, int> p2)
         ->bool{return p1.second > p2.second;});
		return freq;
	}
  string xorer(string& in, char x) {
    string ret;
    for(size_t i = 0; i < in.length(); i++) {
      ret.append(1, in[i]^x);
    }
    return ret;
  }
  double score(string& in) {
    double ret = 0;
    for(auto i = in.begin(); i != in.end(); i++) {
      auto j = freqs.find(*i);
      if(j != freqs.end())
          ret += j->second;
    }
    return ret;
  }
}

pair<double, char> run1(string& t1) {
  string t2 = unhexlify(t1);
  vector<pair<char, int>> freq = frequencer(t2);
  char eta[] = " etaoin";
  pair<double, char> ret;
  ret.first = 0;
  for(auto j = eta; *j; j++) {
    char x = freq[0].first^*j;
    string tmp = xorer(t2, x);
    int val = score(tmp);
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
  pair<double, char> ret;
  ret.first = 0;
  string toret;
  while(!getline(in, t1).eof()) {
    auto res = run1(t1);
    if(res.first > ret.first)
      ret = res,toret=t1;
  }
  printf("%lf\t%c\n%s\n", ret.first, ret.second, toret.c_str());
  toret = unhexlify(toret);
  printf("%s", xorer(toret, ret.second).c_str());
}

int main() {
	run("4.txt");
	return 0;
}
