#include <openssl/conf.h>
#include <openssl/evp.h>
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

aes128_ecb::aes128_ecb(string key) : m_key(key), m_blen(128),
									 m_out(new unsigned char[m_blen]),
									 m_olen(0) {
  if(!(ctx = EVP_CIPHER_CTX_new())) {
     ERR_print_errors_fp(stderr);
	 //	throw
  }
}

void aes128_ecb::checkBuffer(int newlen) {
	if(m_blen <= m_olen + newlen) { //	+ cipher_size
		m_blen *= 2;
		unsigned char *tmp = new unsigned char[m_blen];
		memcpy(tmp, m_out, m_olen);
		delete[] m_out;
		m_out = tmp;
	}
}

void aes128_ecb::e_init() {
	if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_ecb(), NULL,
							   reinterpret_cast<const unsigned char*>(m_key.c_str()),
							   NULL)) {
		ERR_print_errors_fp(stderr);
		//	throw
	}
}

void aes128_ecb::e_append(const string &in) {
	int xlen;
	checkBuffer(in.length());
	if(1 != EVP_EncryptUpdate(ctx, m_out+m_olen, &xlen,
							  reinterpret_cast<const unsigned char*>(in.c_str()),
							  in.length())) {
		ERR_print_errors_fp(stderr);
		//	throw
	}
	m_olen += xlen;
}

string aes128_ecb::e_finalize() {
	int xlen;
	if(1 != EVP_EncryptFinal_ex(ctx, m_out+m_olen, &xlen)) {
		ERR_print_errors_fp(stderr);
		//	throw
	}
	m_olen += xlen;
	m_out[m_olen] = '\0';
	string ret(reinterpret_cast<char*>(m_out));
	delete[] m_out;
	return ret;
}


void aes128_ecb::d_init() {
	if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL,
							   reinterpret_cast<const unsigned char*>(m_key.c_str()),
							   NULL)) {
		ERR_print_errors_fp(stderr);
		//	throw
	}
}

void aes128_ecb::d_append(const string &in) {
	int xlen;
	checkBuffer(in.length());
	if(1 != EVP_DecryptUpdate(ctx, m_out+m_olen, &xlen,
							  reinterpret_cast<const unsigned char*>(in.c_str()),
							  in.length())) {
		ERR_print_errors_fp(stderr);
		//	throw
	}
	m_olen += xlen;
}

string aes128_ecb::d_finalize() {
	int xlen;
	if(1 != EVP_DecryptFinal_ex(ctx, m_out+m_olen, &xlen)) {
		ERR_print_errors_fp(stderr);
		// throw
	}
	m_olen += xlen;
	string ret(reinterpret_cast<char*>(m_out));
	delete[] m_out;
	return ret;
}

#ifdef _AES_STANDALONE_

#include "utils.h"

int main() {
	string input("YELLOW SUBMARINE");
	string pass("aaaaaaaaaaaaaaaa");
	encryptor<aes128_ecb> e(pass);
	string r1 = e.encrypt(input);
	printf("%s\n", hexlify(r1).c_str());
	printf("%s\n", b64enc(r1).c_str());
	decryptor<aes128_ecb> d(pass);
	r1 = d.decrypt(r1);
	printf("%s\n", r1.c_str());
	return 0;
}
#endif
