#include <unistd.h>
#include <string>
#include "encOracle.h"
#include "modeECB.h"
#include "aes128_ecb.h"

using namespace std;

class allMightyOracle : public encOracle {
	string m_key;
	aes128_ecb m_ecb;
	static const string secret;
	public:
	allMightyOracle() : m_ecb(createRandomString(16)) {}
	virtual string encrypt(const string& input) override {
		string data(input);
		data.append(b64dec(secret));
		data = pad(data, 16);
		return m_ecb.encrypt(data);
	}
};

const string allMightyOracle::secret = "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWc"\
									   "tdG9wIGRvd24gc28gbXkgaGFpciBjYW4gYmxvdw"\
									   "pUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZ"\
									   "yBqdXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/"\
									   "IE5vLCBJIGp1c3QgZHJvdmUgYnkK";

int main() {
	srand(time(NULL));
	allMightyOracle oracle;
	string test;
	string cryptxt = oracle.encrypt(test);
	string c1;
	//	discover block size for oracle
	size_t blockLen = discoverBlockLen(oracle);
	log("[>] Cipher block len: %zu", blockLen);
	//	check if ECB
	if(!isECB(oracle, blockLen)) {
		log("[-] Non ECB cipher");
		return 1;
	}
	log("[+] ECB mode discovered");

	string decoded;
	if(!decryptECB(oracle, decoded, blockLen, cryptxt.length())) {
		log("[-] No char to fill found in alphabet!");
		log("[-] Partially decoded:");
	}
	else
		log("[+] Decoded:");
	log("%s", decoded.c_str());

	return 0;
}
