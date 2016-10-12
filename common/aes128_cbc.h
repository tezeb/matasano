#ifndef AES128_CBC_H
#define AES128_CBC_H

#include <string>
#include "xorer.h"
#include "aes128_ecb.h"

class aes128_cbc {
	aes128_ecb m_ecb;
	bool m_encrypt;
	xorer m_mangle;
	std::string m_out;
	public:
	aes128_cbc(const std::string& key="") : m_ecb(key) {
	}
	void init(const std::string& iv, bool encrypt=true);
	void append(const std::string& in);
	std::string finalize();
	std::string encrypt(const std::string& in, const std::string& iv);
	std::string decrypt(const std::string& in, const std::string& iv);
};

#endif //	AES128_CBC_H
