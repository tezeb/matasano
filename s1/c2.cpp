#include <cstdio>
#include "utils.h"
#include "xorer.h"
#include <string>

using namespace std;

void run(string& t1, string& t2, string exp) {	
  xorer x(unhexlify(t2));
  x << unhexlify(t1);
	string res = hexlify(x.get());
	printf("%s\n:%s:\n:%s:\n", res==exp?"MATCH":"FAILED", res.c_str(), exp.c_str());
  printf("%s\n", unhexlify(res).c_str());
}

int main() {
	string t1="1c0111001f010100061a024b53535009181c";
	string t2="686974207468652062756c6c277320657965";
	string t3="746865206b696420646f6e277420706c6179";
	run(t1, t2, t3);
	return 0;
}
