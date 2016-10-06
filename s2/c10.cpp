#include <fstream>
#include "aes128_ecb.h"
#include "utils.h"

using namespace std;

int main() {
	string key = "YELLOW SUBMARINE";
	string filename = "10.txt";
	decryptor<aes128_ecb> d(key);
	ifstream file(filename);
	string tmp1;
	while(!getline(file, tmp1).eof()) {
		tmp1 = b64dec(tmp1);
		print("%d\n", tmp1.length());
	}
	return 0;
}
