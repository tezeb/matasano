#include <fstream>
#include "aes128_cbc.h"
#include "utils.h"

using namespace std;

int main() {
	string key = "YELLOW SUBMARINE";
	string filename = "10.txt";

	ifstream file(filename);
	string data, tmp1;
	while(!getline(file, tmp1).eof()) {
		data.append(tmp1);
	}

	aes128_cbc d(key);
	string iv(16, '\0');
	data = b64dec(data);
	string out = d.decrypt(data, iv);
	log("Decrypted:\n%s\n", out.substr(0, 256).c_str());
	return 0;
}
