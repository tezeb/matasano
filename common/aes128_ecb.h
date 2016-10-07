#ifndef _AES128_ECB_H_
#define _AES128_ECB_H_

#include <string>
#include <openssl/evp.h>

class aes128_ecb {

	EVP_CIPHER_CTX m_ctx;
	size_t m_bufflen;
	unsigned char *m_buff;
	size_t m_outlen;
	const std::string& m_key;
	bool m_encrypt;

	void checkBuffer(size_t newlen);
	public:
	aes128_ecb(const std::string& key) : m_bufflen(128), m_buff(new unsigned char[m_bufflen]),
							   m_outlen(0), m_key(key), m_encrypt(true) {
	}
	~aes128_ecb() {
		delete[] m_buff;
	}
	void init(bool encrypt=true);
	void append(const std::string& in);
	std::string finalize();
};

#endif // _AES128_ECB_H_
