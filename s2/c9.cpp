#include <cstdio>
#include <string>

using namespace std;

string& pad(string& in, char block_size)
{
	char padding = block_size - in.length()%block_size;
	in.append(padding, padding);
	return in;
}

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

void pas20(string& in) {
	pad(in, 20);
	show_hex(in);
}

int main() {
	string t1 = "YELLOW SUBMARINE";
	string t2 = "YELLOW SUBMARIN";
	string t3 = "YELLOW SUBMARI";
	string t4 = "YELLOW SUBMAR";
	string t5 = "YELLOW SUBMA";
	pas20(t1);
	pas20(t2);
	pas20(t3);
	pas20(t4);
	pas20(t5);
	return 0;
}
