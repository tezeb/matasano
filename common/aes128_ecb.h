#ifndef _AES128_ECB_H_
#define _AES128_ECB_H_

#include <string>

class aes128_ecb {
	const std::string& m_key;
	EVP_CIPHER_CTX *ctx;
	int m_blen;
	unsigned char *m_out;
	int m_olen;
	private:
		void checkBuffer(int newlen);
	public:
		aes128_ecb(std::string key);
		void e_init();
		void e_append(const std::string& in);
		std::string e_finalize();
		void d_init();
		void d_append(const std::string& in);
		std::string d_finalize();
};

template<class cipher>
class encryptor {
	cipher m_enc;
	public:
		encryptor(const std::string& key) : m_enc(key) {
			m_enc.e_init();
		}
		void append(const std::string& in) {
			m_enc.e_append(in);
		}
		std::string encrypt(const std::string& in) {
			append(in);
			return finalize();
		}
		std::string finalize() {
			return m_enc.e_finalize();
		}
};

template<class cipher>
class decryptor {
	cipher m_dec;
	public:
		decryptor(const std::string& key) : m_dec(key) {
			m_dec.d_init();
		}
		void append(const std::string& in) {
			m_dec.d_append(in);
		}
		std::string decrypt(const std::string& in) {
			append(in);
			return finalize();
		}
		std::string finalize() {
			return m_dec.d_finalize();
		}
};

#endif // _AES128_ECB_H_
