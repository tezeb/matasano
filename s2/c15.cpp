#include <cstdio>
#include <string>
#include "utils.h"

using namespace std;

void test(string& tmp) {
	try {
		unpad(tmp);
		log("[+] %s", hexNonPrint(tmp).c_str());
	} catch(InvalidPadding& e) {
		log("[-] (%s): %s", e.what(), hexNonPrint(tmp).c_str());
	}
}

int main() {
	string t1 = "YELLOW SUBMARINE";
	int j = 0;
    for(int i = 10; i < 25; i++) {
        string tmp(t1);
        pad(tmp, i);
		test(tmp);
        pad(tmp, i);
		tmp.erase(tmp.length() - j);
		test(tmp);
		j%=10;
		j++;
    }
	string t2 = "ICE ICE BABY\x04\x04\x04\x04";
	string t3 = "ICE ICE BABY\x05\x05\x05\x05";
	string t4 = "ICE ICE BABY\x01\x02\x03\x04";
	test(t2);
	test(t3);
	test(t4);

	return 0;
}
