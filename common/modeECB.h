#ifndef MODE_ECB_H
#define MODE_ECB_H

#include <string>
#include "encOracle.h"

namespace ECB_data {
	extern const std::string printable;
}

std::string findMaxRepeatedChunk(const std::string& tmp, int chunkLen=16);

int countRepetitions(const std::string& tmp, int chunkLen=16);

bool isECB(encOracle& oracle, int blockLen, int repeat=3);

bool decryptECB(encOracle& oracle, std::string& decoded, size_t blockLen,
				size_t plaintextLen, const std::string& alphabet=ECB_data::printable);

size_t discoverBlockLen(encOracle& oracle, size_t expected = 16,
						int testAmount = 64, bool verbose=false);

#endif 	//	MODE_ECB_H
