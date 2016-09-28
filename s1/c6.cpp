#include <cstdio>
#include "utils.h"
#include <string>
#include "xor_utils.h"

using namespace std;

void run(string& t1, string& t2, int exp) {	
  int r = editDist(t1, t2);
  printf("%s:\t%d == %d\n", r==exp?"MATCH":"FAILED", r, exp);
}

int main() {
	string t1="this is a test";
	string t2="wokka wokka!!!";
	run(t1, t2, 37);
  string f = b64dec(readFile("6.txt"));
  int min_keys = -1;
  double min_dist = -1;
  for(int i = 2; i < 60; i++) {
    double dist = 0;
    double eDist = 0;
    int wordsCompAmount = 8;
    for(int j = 0; j < 2*wordsCompAmount; j+=2) {
      string s1 = f.substr(j*i, i);
      string s2 = f.substr((j+1)*i, i);
//      printf("%s\n", hexlify(s1).c_str());
//      printf("%s\n", hexlify(s2).c_str());
      eDist = editDist(s1, s2);
      dist += eDist/i;
    }
    dist /= wordsCompAmount;
    if(min_dist > dist || min_dist == -1) {
      min_dist = dist;
      min_keys = i;
    }
  }
  printf("Min keys: %d, min dist: %lf\n", min_keys, min_dist);
  string key;
  for(int i = 0; i < min_keys; i++) {
    string dec;
    for(size_t j = i; j < f.length(); j+=min_keys) {
      dec += f[j];
    }
    auto res = bruteXor_eng_char(dec);
//    printf("> %d\n", res.second);
    key.append(1, res.first[0].c);
  }
  string res = (xorer(key)<<f).get();
  printf("Decrypted:\n%s\n", res.substr(0, 256).c_str());
	return 0;
}
