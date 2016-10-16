#include <unistd.h>
#include <string>
#include "aes128_ecb.h"
#include "encOracle.h"
#include "modeECB.h"

using namespace std;

class allMightyOracle : public encOracle {
	string m_key;
	aes128_ecb m_ecb;
	size_t m_pSize;
	static const string secret;
	public:
	allMightyOracle(string key = createRandomString(16), size_t prefixSize=2048) :
					m_ecb(key), m_pSize(prefixSize) {}
	allMightyOracle(size_t prefixSize) : allMightyOracle(createRandomString(16), prefixSize) {}
	virtual string encrypt(const string& input) override {
		string data;
		if(m_pSize)
			data.append(createRandomString(rand()%m_pSize));
		data.append(input);
		data.append(b64dec(secret));
		data = pad(data, 16);
		string x= m_ecb.encrypt(data);
		return x;
	}
};

const string allMightyOracle::secret = "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWc"\
									   "tdG9wIGRvd24gc28gbXkgaGFpciBjYW4gYmxvdw"\
									   "pUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZ"\
									   "yBqdXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/"\
									   "IE5vLCBJIGp1c3QgZHJvdmUgYnkK";

int main() {
	srand(time(NULL));
	allMightyOracle oracle(16384);
	string cryptxt = oracle.encrypt("");

	size_t blockLen = discoverBlockLen(oracle);
	log("[>] Cipher block len: %zu", blockLen);

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
