#include "aes128_ctr.h"
#include "utils.h"

using namespace std;

void aes128_ctr::init(unsigned long long bcnt) {
	h.rk.m_bcnt = bcnt;
	m_x.reset();
	m_i = 0;
}

void aes128_ctr::append(const std::string &in) {
	size_t i = 0;
	i = m_i%16;
	if(i != 0) {
		i = 16-i;
		m_x << in.substr(0, i);
	}
	while(i < in.length()) {
		m_x.setKey(getKey()) << in.substr(i, 16);
		i += 16;
	}
	m_i += in.length();
}

string aes128_ctr::getKey() {
	string ret(h.pkey, 16);
	h.rk.m_bcnt++;
	return m_ecb.encrypt(ret);
}

string aes128_ctr::finalize() {
	return m_x.get();
}

string aes128_ctr::encrypt(const string& in, unsigned long long bcnt) {
	init(bcnt);
	append(in);
	return finalize();
}

string aes128_ctr::decrypt(const string& in, unsigned long long bcnt) {
	return encrypt(in, bcnt);
}
