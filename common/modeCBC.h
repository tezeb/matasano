#ifndef MODE_CBC_H
#define MODE_CBC_H

#include <string>
#include "utils.h"

class cbcOracle {
	public:
		//	It is expected that this will throw InvalidPadding
		virtual void consumeSecret(const std::string&) = 0;
};

std::string decryptBlock(cbcOracle& oracle, const std::string& block);

std::string decryptCBCPaddingOracle(cbcOracle& oracle, std::string& cryptxt, size_t blockLen=16);


#endif 	//	MODE_CBC_H
