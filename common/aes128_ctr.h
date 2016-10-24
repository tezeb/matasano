#ifndef _AES128_CTR_H_
#define _AES128_CTR_H_

#include "aes128_ecb.h"
#include "xorer.h"

class aes128_ctr {
	aes128_ecb m_ecb;
	union {
		char pkey[16];
		struct {
			unsigned long long m_nonce;
			unsigned long long m_bcnt;
		} rk;
	} h;
	xorer m_x;
	size_t m_i;
	std::string getKey();
	public:
	aes128_ctr(const std::string& key, unsigned long long nonce=0) :
		m_ecb(key), h(), m_i(0) {
			h.rk.m_nonce = nonce;
	}
	~aes128_ctr() {
	}
	void init(unsigned long long bcnt=0);
	void append(const std::string& in);
	std::string finalize();
	std::string encrypt(const std::string& in, unsigned long long bcnt=0);
	std::string decrypt(const std::string& in, unsigned long long bcnt=0);
};

#endif // _AES128_CTR_H_
