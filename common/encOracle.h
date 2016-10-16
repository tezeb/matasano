#ifndef ENC_ORACLE_H
#define ENC_ORACLE_H

#include <string>

class encOracle {
	public:
	virtual std::string encrypt(const std::string& input) = 0;
	virtual ~encOracle() {};
};

#endif //	ENC_ORACLE_H
