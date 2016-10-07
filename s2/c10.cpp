#include <fstream>
#include "aes128_ecb.h"
#include "utils.h"
#include "xorer.h"

using namespace std;

int main() {
	string key = "YELLOW SUBMARINE";
	string filename = "10.txt";

	ifstream file(filename);
	string data, tmp1;
	while(!getline(file, tmp1).eof()) {
		data.append(tmp1);
	}

	aes128_ecb d(key);
	string iv(16, '\0');
	xorer mangle(iv);
	data = b64dec(data);
	string out;
	for(size_t i = 16; i <= data.length(); i+=16) {
		string cryptext = data.substr(i-16, 16);
		string middleform = d.decrypt(cryptext);
		out.append((mangle << middleform).get());
		mangle.reset(cryptext);
	}
	log("Decrypted:\n%s\n", out.substr(0, 256).c_str());
	return 0;
}
