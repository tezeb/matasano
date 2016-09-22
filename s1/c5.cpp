#include <cstdio>
#include "utils.h"
#include <string>
#include "xorer.h"

using namespace std;

void run(string& t1, string& t2, string exp) {	
  xorer x(t2);
  string res = (x<<t1)();
  string hres = hexlify(res);
  printf("%s\n:%s:\n:%s:\n", hres==exp?"MATCH":"FAILED", hres.c_str(), exp.c_str());
  x.reset();
  x<<res;
  hres=x();
  printf("%s\n:%s:\n:%s:\n", hres==t1?"MATCH":"FAILED", hres.c_str(), t1.c_str());
}

int main() {
	string t1="Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";
	string t2="ICE";
  string t3="0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f";
	run(t1, t2, t3);
	return 0;
}
