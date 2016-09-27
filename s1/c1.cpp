#include <string>
#include "utils.h"

using namespace std;

void run(string& t1, string& exp) {	
	string ret = hex2b64(t1, t1.length());
	printf("%s\n:%s:\n:%s:\n", ret==exp?"MATCH":"FAILED", ret.c_str(), exp.c_str());
}

int main() {
	string t1="49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
  string t2="SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";
	/* test padding
	hex2b64(t1, 8);
	hex2b64(t1, 6);
	hex2b64(t1, 4);
	hex2b64(t1, 2);
	hex2b64(t1, sizeof(t1)/sizeof(char)-3);
	hex2b64(t1, sizeof(t1)/sizeof(char)-5);
	hex2b64(t1, sizeof(t1)/sizeof(char)-7);
	*/
  run(t1, t2);
  return 0;
}
