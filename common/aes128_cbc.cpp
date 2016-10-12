#include "aes128_cbc.h"

using namespace std;

void aes128_cbc::init(const string& iv, bool encrypt) {
	m_mangle.reset(iv);
	m_ecb.init(encrypt);
	m_encrypt = encrypt;
	m_out.clear();
}

void aes128_cbc::append(const string& in) {
	//	add check for length%16
	for(size_t i = 16; i <= in.length(); i+=16) {
		string input = in.substr(i-16, 16);
		string output;
		if(m_encrypt) {
			output = m_ecb.encrypt((m_mangle << input).get());
			m_mangle.reset(output);
		}
		else {
			output = (m_mangle << m_ecb.decrypt(input)).get();
			m_mangle.reset(input);
		}
		m_out.append(output);
	}
}

std::string aes128_cbc::finalize() {
	return m_out;
}

std::string aes128_cbc::encrypt(const string& in, const string& iv) {
	string out;
	init(iv, true);
	append(in);
	return finalize();
}

std::string aes128_cbc::decrypt(const string& in, const string& iv) {
	string out;
	init(iv, false);
	append(in);
	return finalize();
}

