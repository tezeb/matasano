#include <cstdio>
#include <string>
#include "utils.h"

using namespace std;

int main() {
	string t1 = "YELLOW SUBMARINE";
	string t2 = "YELLOW SUBMARIN";
	string t3 = "YELLOW SUBMARI";
	string t4 = "YELLOW SUBMAR";
	string t5 = "YELLOW SUBMA";
    for(int i = 10; i < 25; i++) {
        string tmp(t1);
        pad(tmp, i);
        log("%d (%zu): %s", i, tmp.length(), hexNonPrint(tmp).c_str());
    }
	return 0;
}
