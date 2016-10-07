#include <openssl/conf.h>
#include <openssl/err.h>
#include <cstring>
#include "aes128_ecb.h"

using namespace std;

namespace {

	static struct ssl_static_initializer {
		ssl_static_initializer() {
			/* Load the human readable error strings for libcrypto */
			ERR_load_crypto_strings();

			/* Load all digest and cipher algorithms */
			OpenSSL_add_all_algorithms();

			/* Load config file, and other important initialisation */
			OPENSSL_config(NULL);
		}
		~ssl_static_initializer() {
			/* Removes all digests and ciphers */
			EVP_cleanup();

			/* if you omit the next, a small leak may be left when you make use of the BIO (low level API) for e.g.
			 * base64 transformations */
			CRYPTO_cleanup_all_ex_data();

			/* Remove error strings */
			ERR_free_strings();
		}
	} internal_ssl_static_initializer;

}

void aes128_ecb::init(bool encrypt) {
	m_outlen = 0;
	m_encrypt = encrypt;

	EVP_CIPHER_CTX_init(&m_ctx);
	if(1 != EVP_CipherInit_ex(&m_ctx, EVP_aes_128_ecb(), NULL, NULL, NULL, m_encrypt)) {
		ERR_print_errors_fp(stderr);
		EVP_CIPHER_CTX_cleanup(&m_ctx);
		throw 13;
	}
	EVP_CIPHER_CTX_set_padding(&m_ctx, 0);

	/* Now we can set key and IV */
	if(1 != EVP_CipherInit_ex(&m_ctx, NULL, NULL,
							  reinterpret_cast<const unsigned char*>(m_key.c_str()),
							  NULL, -1)) {
		ERR_print_errors_fp(stderr);
		EVP_CIPHER_CTX_cleanup(&m_ctx);
		throw 13;
	}
}

void aes128_ecb::append(const std::string &in) {
	int tmp;

	checkBuffer(in.length());

	if(1 != EVP_CipherUpdate(&m_ctx, m_buff+m_outlen, &tmp,
							 reinterpret_cast<const unsigned char*>(in.c_str()),
							 in.length())) {
		ERR_print_errors_fp(stderr);
		EVP_CIPHER_CTX_cleanup(&m_ctx);
		throw 13;
	}
	m_outlen += tmp;
}

string aes128_ecb::finalize() {
	int xlen;

	if(1 != EVP_CipherFinal_ex(&m_ctx, m_buff+m_outlen, &xlen)) {
		ERR_print_errors_fp(stderr);
		EVP_CIPHER_CTX_cleanup(&m_ctx);
		throw 13;
	}
	m_outlen += xlen;
	m_buff[m_outlen] = '\0';

	string ret(reinterpret_cast<const char*>(m_buff), m_outlen);

	EVP_CIPHER_CTX_cleanup(&m_ctx);

	m_outlen = 0;
	return ret;
}

void aes128_ecb::checkBuffer(size_t newlen) {
	if(m_bufflen <= m_outlen + newlen) { //	+ cipher_size
		while(m_bufflen <= m_outlen + newlen)
			m_bufflen *= 2;
		unsigned char *tmp = new unsigned char[m_bufflen];
		memcpy(tmp, m_buff, m_outlen);
		delete[] m_buff;
		m_buff = tmp;
	}
}

string aes128_ecb::encrypt(const string& in) {
	init();
	append(in);
	return finalize();
}

string aes128_ecb::decrypt(const string& in) {
	init(false);
	append(in);
	return finalize();
}

#ifdef AES_STANDALONE

#include "utils.h"

void testCrypt(string in, string& key) {
	aes128_ecb x(key);
	x.init();
	for(int i = 0; i < 100; i++)
		x.append(in);
	string ret = x.finalize();

	for(int j = 0; j < 5; j++) {
		x.init();
		x.append(ret);
		ret = x.finalize();
	}
	for(int j = 0; j < 6; j++) {
		x.init(false);
		x.append(ret);
		ret = x.finalize();
	}
	if (ret.length() != 100*in.length())
		log("!!!!!!!!!!!!!!!! ERROR !!!!!!!!!!!!!!!!");

}

int main() {
	string input("YELLOW SUBMARINE");
	string pass("aaaaaaaaaaaaaaaa");
	log("%s", hexlify(input).c_str());
	testCrypt(input, pass);
	return 0;
}

#endif
