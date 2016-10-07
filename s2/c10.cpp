#include <fstream>
#include "aes128_ecb.h"
#include "utils.h"
#include "xorer.h"

#define log(...) fprintf(stderr, __VA_ARGS__)

using namespace std;

int main() {
	string key = "YELLOW SUBMARINE";
	string filename = "10.txt";
	decryptor<aes128_ecb> d(key);
	ifstream file(filename);
	string data, tmp1;
	string iv("\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0");
	xorer mangle(iv);
	while(!getline(file, tmp1).eof()) {
		data.append(tmp1);
	}
	log("%s\n", data.c_str());
	log("%zu\n", data.length());
				/*
			for(size_t i = 0; i < data.length(); i+=16)
			{
				string dec1 = d.decrypt(data.substr(i*16, (i+1)*16));
				string out = (mangle << dec1).get();
				log("%zu: %s\n", dec1.length(), dec1.c_str());
				mangle.reset();
				mangle.setKey(dec1);
				d.reinit();
			}
				*/
	/*
	data = b64dec(data);
	log("%s\n", data.c_str());
	log("%zu\n", data.length());
	string dec1 = d.decrypt(data);
	log("%zu: %s\n", dec1.length(), dec1.c_str());
	data.clear();
	*/
	string pass("YELLOW SUBMARINE");
	string input("aaaaaaaaaaaaaaaa");
	encryptor<aes128_ecb> e(pass);
	string r1 = e.encrypt(input);
	log("%s %zu\n", hexlify(r1).c_str(), r1.length());
	log("%s\n", b64enc(r1).c_str());
	d.reinit();
	r1 = d.decrypt(r1);
	log("%s\n", r1.c_str());

	return 0;
}
