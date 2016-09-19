#include <cstdio>
#include "utils.h"
#include <string>

using namespace std;

namespace {
	string xorer(const string& a, const string& b) {
		string ret;
		for(size_t i = 0; i+1 < a.length(); i+=2) {
			char f = h2c(a[i], a[i+1]);
			char g = h2c(b[i], b[i+1]);
			ret.append(1, f^g);
		}
		return ret;
	}
}

void run(string& t1, string& t2, string exp) {	
	string res = xorer(t1, t2);
	string hres = hexlify(res);
	string uexp = unhexlify(exp);
	printf("%s\n:%s:\n:%s:\n", hres==exp?"MATCH":"FAILED", hexlify(res).c_str(), exp.c_str());
	printf("%s\n:%s:\n:%s:\n", res==uexp?"MATCH":"FAILED", res.c_str(), unhexlify(exp.c_str()).c_str());
}

int main() {
	string t1="1c0111001f010100061a024b53535009181c";
	string t2="686974207468652062756c6c277320657965";
	string t3="746865206b696420646f6e277420706c6179";
	run(t1, t2, t3);
	return 0;
}
