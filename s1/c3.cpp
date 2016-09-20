#include <cstdio>
#include "utils.h"
#include <string>
#include <map>
#include <algorithm>
#include <vector>

using namespace std;

namespace {
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
}

void run(string& t1) {
  string t2 = unhexlify(t1);
/*
  for(int c = 0; c < 256; c++) {
    string res = xorer(t1, c);
    char t[] = "ETAOIN SHRDLU";
    auto v = frequencer(res);
    for(auto i = v.begin(); i != v.end(); i++) {
    }
  }
  */
}

void run1(string& t1) {
  string t2 = unhexlify(t1);
  vector<pair<char, int>> freq = frequencer(t2);
  char x = freq[0].first^' ';
  for(auto i = freq.begin(); i != freq.end(); i++) {
    printf("%c(%d), ", (i->first^x), i->second);
  }
  printf("\n>%s\n", xorer(t2, freq[0].first^' ').c_str());
//  printf("%s\n", xorer(t1, freq[0].first^'E').c_str());
//	printf("%s\n:%s:\n:%s:\n", hres==exp?"MATCH":"FAILED", hexlify(res).c_str(), exp.c_str());
//	printf("%s\n:%s:\n:%s:\n", res==uexp?"MATCH":"FAILED", res.c_str(), unhexlify(exp.c_str()).c_str());
}

int main() {
	string t1="1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
	run1(t1);
	return 0;
}
