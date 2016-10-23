#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include "utils.h"
#include "aes128_cbc.h"
#include "xorer.h"
#include "modeCBC.h"

using namespace std;

class service : public cbcOracle {
	aes128_cbc m_cbc;	
	static const vector<string> secrets;
	vector<string>::const_iterator m_next;
	public:
	service() : m_cbc(createRandomString(16)), m_next(secrets.cbegin()) {
	}
	string getNextSecret() {
		if (m_next == secrets.end())
			return "";
		string tmp = createRandomString(16);
		string data = b64dec(*m_next++);
		tmp += m_cbc.encrypt(pad(data, 16), tmp);
		return tmp;
	}
	void consumeSecret(const string& cryptxt) {
		string iv = cryptxt.substr(0, 16);
		string p = m_cbc.decrypt(cryptxt.substr(16), iv);
		unpad(p);
	}
};

const vector<string> service::secrets = {
	"MDAwMDAwTm93IHRoYXQgdGhlIHBhcnR5IGlzIGp1bXBpbmc=",
	"MDAwMDAxV2l0aCB0aGUgYmFzcyBraWNrZWQgaW4gYW5kIHRoZSBWZWdhJ3MgYXJlIHB1bXBpbic=",
	"MDAwMDAyUXVpY2sgdG8gdGhlIHBvaW50LCB0byB0aGUgcG9pbnQsIG5vIGZha2luZw==",
	"MDAwMDAzQ29va2luZyBNQydzIGxpa2UgYSBwb3VuZCBvZiBiYWNvbg==",
	"MDAwMDA0QnVybmluZyAnZW0sIGlmIHlvdSBhaW4ndCBxdWljayBhbmQgbmltYmxl",
	"MDAwMDA1SSBnbyBjcmF6eSB3aGVuIEkgaGVhciBhIGN5bWJhbA==",
	"MDAwMDA2QW5kIGEgaGlnaCBoYXQgd2l0aCBhIHNvdXBlZCB1cCB0ZW1wbw==",
	"MDAwMDA3SSdtIG9uIGEgcm9sbCwgaXQncyB0aW1lIHRvIGdvIHNvbG8=",
	"MDAwMDA4b2xsaW4nIGluIG15IGZpdmUgcG9pbnQgb2g=",
	"MDAwMDA5aXRoIG15IHJhZy10b3AgZG93biBzbyBteSBoYWlyIGNhbiBibG93"
};

int main() {
	srand(time(NULL));
	service o;
	string tmp;
	while(!(tmp = o.getNextSecret()).empty()) {
		log("%s", decryptCBCPaddingOracle(o, tmp).c_str());
	}
	return 0;
}
