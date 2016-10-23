#include "modeCBC.h"
#include "utils.h"
#include "xorer.h"

using namespace std;

string decryptBlock(cbcOracle& oracle, const string& block) {
	size_t blockLen = block.length();
	string middle(blockLen, '\0');
	string fake(blockLen, 'A');
	fake.append(block);
	unsigned char pad = 1;
	for(int j = block.length() - 1; j >= 0; j--) {
		bool found = false;
		for(int c = 0; c < 256; c++) {
			fake[j] = c;
			try {
				oracle.consumeSecret(fake);
				middle[j] = pad ^ c;
				pad++;
				for(size_t k = j; k < block.length(); k++) {
					fake[k] = middle[k] ^ pad;
				}
				found = true;
				break;

			} catch(InvalidPadding& e) {
			}
		}
		if(!found) {
			log("[-] Error");
			break;
		}
	}
	return middle;
}

string decryptCBCPaddingOracle(cbcOracle& oracle, string& cryptxt, size_t blockLen) {
	xorer x;
	for(size_t i = 2*blockLen; i <= cryptxt.length(); i+=blockLen) {
		x.setKey(cryptxt.substr(i-2*blockLen, blockLen));
		x << decryptBlock(oracle, cryptxt.substr(i-blockLen, blockLen));
	}
	string ret = x.get();
	return unpad(ret);
}

