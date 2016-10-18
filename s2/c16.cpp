#include <unistd.h>
#include <string>
#include <sstream>
#include <stdexcept>
#include "aes128_cbc.h"
#include "modeECB.h"
#include <functional>

using namespace std;

class service {
	string m_key;
	aes128_cbc m_cbc;
	string m_iv;
	string encrypt(string input) {
		pad(input, 16);
		return m_cbc.encrypt(input, m_iv);
	}
	string decrypt(const string& input) {
		string data = m_cbc.decrypt(input, m_iv);
		unpad(data);
		return data;
	}
	public:
	service() : m_cbc(createRandomString(16)), m_iv(createRandomString(16)) {}
	string createProfile(const string& userdata) {
		ostringstream out;
		out << "comment1=cooking%20MCs;userdata=";
		out << url_encode(userdata);
		out << ";comment2=%20like%20a%20pound%20of%20bacon";
		return encrypt(out.str());
	}
	bool logIn(const string& cookie) {
		string data = decrypt(cookie);
		log("----------------\n%s\n-----", hexNonPrint(data).c_str());
		map<string, string> userInfo = splitUrlValues(data);
		for(auto i = userInfo.begin(); i != userInfo.end(); i++) {
			log("%s:\t%s", i->first.c_str(), hexNonPrint(url_decode(i->second)).c_str());
		}
		try {
			return userInfo.at("admin") == "true";
		} catch (out_of_range& e) {
		}
		return false;
	}
};

void tryInput(service& oracle, string input, function<void(string&)> fix = nullptr) {
	string c1 = oracle.createProfile(input);
	if(fix)
		fix(c1);
	if(oracle.logIn(c1))
		log("[+] You're admin!");
	else
		log("[-] Just a user :(");
}

int main() {
	service oracle;
	tryInput(oracle, "So lovely, I can store data here");
	tryInput(oracle, "test@test.com;admin=true");
	tryInput(oracle, string(16, 'A').append(5, 'B').append("3admin5true"),
			 [](string& data) -> void {data[37] ^= '3' ^ ';'; data[43] ^= '5' ^ '=';});
	return 0;
}
