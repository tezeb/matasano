#include <cstdio>
#include <string>
#include "utils.h"

using namespace std;

void show_hex(string& in) {
	string out;
	out.reserve(3*in.length());
	for(char& i: in) {
		if(isprint(i))
			out.push_back(i);
		else {
			char buf[5];
			snprintf(buf, 5, "\\x%02X", i);
			out.append(buf);
		}
	}
	printf("%02zu:\t%s\n", in.length(), out.c_str());
}

int main() {
	string t1 = "YELLOW SUBMARINE";
	string t2 = "YELLOW SUBMARIN";
	string t3 = "YELLOW SUBMARI";
	string t4 = "YELLOW SUBMAR";
	string t5 = "YELLOW SUBMA";
    for(int i = 10; i < 25; i++) {
        string tmp(t1);
        pad(tmp, i);
        show_hex(tmp);
    }
	return 0;
}
