#include <unistd.h>
#include <cstdio>
#include <fstream>
#include <string>
#include "utils.h"
#include "aes128_ecb.h"
#include "aes128_cbc.h"
#include <sstream>
#include <map>
#include <stdexcept>

using namespace std;

class service {
	string m_key;
	aes128_ecb m_ecb;
	string encrypt(string input) {
		pad(input, 16);
		return m_ecb.encrypt(input);
	}
	string decrypt(const string& input) {
		string data = m_ecb.decrypt(input);
		data.erase(data.length()-data[data.length()-1]);
		return data;
	}
	map<string, string> splitUrlValues(const string& cookie) {
		map<string, string> ret;
		size_t pos = 0;
		size_t prev_pos = 0;
		while((pos = cookie.find_first_of("&=", prev_pos)) != string::npos) {
			string key = cookie.substr(prev_pos, pos-prev_pos);
			string value;
			prev_pos = pos + 1;
			if(cookie[pos] == '=') {
				if ((pos = cookie.find_first_of('&', prev_pos)) == string::npos) {
					pos = cookie.length();
				}
				value = cookie.substr(prev_pos, pos-prev_pos);
				prev_pos = pos + 1;
			}
			ret.insert(pair<string, string>(key, value));
		}
		if(prev_pos < cookie.length())
			ret.insert(pair<string, string>(cookie.substr(prev_pos, pos-prev_pos), string()));
		return ret;
	}
	public:
	service() : m_ecb(createRandomString(16)) {}
	string createProfile(const string& email) {
		ostringstream out;
		out << "email=" << url_encode(email);
		out << "&uid=" << rand()%1000000;
		out << "&role=user";
		return encrypt(out.str());
	}
	bool logIn(const string& cookie) {
		string data = decrypt(cookie);
		log("%s", data.c_str());
		map<string, string> userInfo = splitUrlValues(data);
		log("----------------");
		for(auto i = userInfo.begin(); i != userInfo.end(); i++) {
			log("%s:\t%s", i->first.c_str(), url_decode(i->second).c_str());
		}
		try {
			return userInfo.at("role") == "admin";
		} catch (out_of_range& e) {
		}
		return false;
	}
};

bool isECB(const string& cryptxt) {
	return (countRepetitions(cryptxt) > 1);
}

int main() {
	srand(time(NULL));
	service oracle;
	string test;
	string c1;
	test = "test@test.com";
	c1 = oracle.createProfile(test);
	oracle.logIn(c1);
	test = "test@test.com&role=admin";
	c1 = oracle.createProfile(test);
	oracle.logIn(c1);
	test = "";
	string cryptxt = oracle.createProfile(test);
	log("%zu", cryptxt.length());
	//	discover block size for oracle
	do {
		test.append(1, 'A');
		c1 = oracle.createProfile(test);
	} while(c1.length() == cryptxt.length());
	size_t blockLen = c1.length() - cryptxt.length();
	log("[>] Cipher block len: %zu", blockLen);
	//	check if ECB
	test = string(3*blockLen, 'A');
	if(!isECB(test)) {
		log("[-] Non ECB cipher");
		return 1;
	}
	log("[+] ECB mode discovered");
	//	No email validation so we can write anything there
	//	chunk "admin&uid=XXXXXX"
	test.clear();
	test.append(16-string("email=").length(), 'a');
	test.append("admin");
	c1 = oracle.createProfile(test).substr(16, 16);
	//	get valid padding
	test.append(7, 'A');
	string p1;
	do {
		p1 = oracle.createProfile(test).substr(48);
	} while(p1.length() < 16);
	c1.append(p1);
	//	chunk "uid=XXXXXX&role="
	test.clear();
	test.append(16-string("email=&").length(), 'a');
	string c2;
	do {
		c2 = oracle.createProfile(test).substr(0, 32);
		c2.append(c1);
	} while(!oracle.logIn(c2));
	return 0;
}
