#include <unistd.h>
#include <cstdio>
#include <fstream>
#include <string>
#include "utils.h"
#include "aes128_ecb.h"
#include "aes128_cbc.h"

using namespace std;

class allMightyOracle {
	string m_key;
	aes128_ecb m_ecb;
	public:
	allMightyOracle() : m_ecb(createRandomString(16)) {}
	string encrypt(const string& input) {
		string secret("Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkgaGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK");
		string data(input);
		data.append(b64dec(secret));
		data = pad(data, 16);
		return m_ecb.encrypt(data);
	}
};

bool isECB(const string& cryptxt) {
	return (countRepetitions(cryptxt) > 1);
}

int main() {
	string alphabet = " 0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRS\
TUVWXYZ!\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~\t\n\r\x0b\x0c";
	srand(time(NULL));
	allMightyOracle oracle;
	string test;
	string cryptxt = oracle.encrypt(test);
	string c1;
	//	discover block size for oracle
	do {
		test.append(1, 'A');
		c1 = oracle.encrypt(test);
	} while(c1.length() == cryptxt.length());
	size_t blockLen = c1.length() - cryptxt.length();
	size_t paddingLen = test.length();
	log("[>] Cipher block len: %zu", blockLen);
	log("[>] Padding len: %zu", paddingLen);
	//	check if ECB
	test = string(3*blockLen, 'A');
	if(!isECB(test)) {
		log("[-] Non ECB cipher");
		return 1;
	}
	log("[+] ECB mode discovered");
	int fillLength = blockLen - 1;
	string decoded(fillLength, 'A');
	do {
		//	fillLength of bytes is needed to guess last byte
		int dl = decoded.length() - fillLength;
		//	length of prefix string is shrinking with more
		//	characters that are decrypted(so (blockLen-1)-dl % blockLen
		//	which just mean amount of characters missing to fill block
		test = decoded.substr(dl, ((blockLen-1)-dl%blockLen));
		c1 = oracle.encrypt(test).substr(dl/blockLen*blockLen, blockLen);
		char result = 0;
		for(char& t : alphabet) {
			string tryMe = decoded.substr(decoded.length()-fillLength) + t;
			string c2 = oracle.encrypt(tryMe).substr(0, blockLen);
			if(c1 == c2) {
				result = t;
				break;
			}
		}
		if(result == 0) {
			log("[-] No char to fill found in alphabet!");
			log("[>] Result: %s", decoded.substr(15).c_str());
			return 1;
		}
		decoded.append(1, result);
	} while(cryptxt.length() > decoded.length() + paddingLen - 15);
    log("[>] Decoded:");
	log("%s", decoded.substr(fillLength).c_str());
	return 0;
}
