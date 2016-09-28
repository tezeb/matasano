#include <string>
#include "utils.h"

using namespace std;

void run(string& t1, string& exp) {	
	string ret = hex2b64(t1, t1.length());
	printf("%s\n:%s:\n:%s:\n", ret==exp?"MATCH":"FAILED", ret.c_str(), exp.c_str());
}

void run1(string& t1, string& exp) {	
	string ret = hexlify(b64dec(t1));
	printf("%s\n:%s:\n:%s:\n", ret==exp?"MATCH":"FAILED", ret.c_str(), exp.c_str());
}

void run2(string& t1) {
  printf("%s\n", hex2b64(t1, 8).c_str());
  printf("%s\n", hex2b64(t1, 6).c_str());
  printf("%s\n", hex2b64(t1, 4).c_str());
  printf("%s\n", hex2b64(t1, 2).c_str());
  printf("%s\n", hex2b64(t1, sizeof(t1)/sizeof(char)-3).c_str());
  printf("%s\n", hex2b64(t1, sizeof(t1)/sizeof(char)-5).c_str());
  printf("%s\n", hex2b64(t1, sizeof(t1)/sizeof(char)-7).c_str());
}

int main() {
	string t1="49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
  string t2="SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
  run(t1, t2);
  run1(t2, t1);
  run2(t1);
  return 0;
}
