#include <cstdio>
#include <algorithm>
#include <sstream>
#include <set>
#include "modeECB.h"
#include "utils.h"

using namespace std;

const string ECB_data::printable(" 0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRS"\
								 "TUVWXYZ!\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~\t\n\r\x0b\x0c");

string findMaxRepeatedChunk(const string& tmp, int chunkLen) {
	int maxCnt = 0;
	size_t off = string::npos;
	for(size_t i = 0; i < tmp.length(); i+=chunkLen) {
		int cnt = 1;
		for(size_t j = i+chunkLen; j < tmp.length(); j+=chunkLen) {
			if(tmp.substr(i, chunkLen) == tmp.substr(j, chunkLen))
				cnt++;
		}
		if(cnt >  maxCnt) {
			maxCnt = cnt;
			off = i;
		}
	}
	return tmp.substr(off, chunkLen);
}

int countRepetitions(const string& tmp, int chunkLen) {
	int maxCnt = 0;
	for(size_t i = 0; i < tmp.length(); i+=chunkLen) {
		int cnt = 1;
		for(size_t j = i+chunkLen; j < tmp.length(); j+=chunkLen) {
			if(tmp.substr(i, chunkLen) == tmp.substr(j, chunkLen))
				cnt++;
		}
		if(cnt >  maxCnt)
			maxCnt = cnt;
	}
	return maxCnt;
}

bool isECB(encOracle& oracle, int blockLen, int repeat) {
	string test(repeat*blockLen, 'A');
	string cryptxt = oracle.encrypt(test);
	return (countRepetitions(cryptxt) > 1);
}

bool decryptECB(encOracle& oracle, string& decoded, size_t blockLen,
				size_t plaintextLen, const string& alphabet)
{
	string mark(blockLen*8, 'A');
	string encMark = findMaxRepeatedChunk(oracle.encrypt(mark));
	mark.erase(blockLen);
	string bMark(blockLen*8, 'B');
	encMark.append(findMaxRepeatedChunk(oracle.encrypt(bMark)));
	bMark.erase(blockLen);
	mark.append(bMark);

	int fillLength = blockLen - 1;
	if(decoded.empty())
		decoded.append(fillLength, 'A');

	bool ret = true;
	int tries = 0;
	do {
		fprintf(stderr, "\rDecoded: %zu/%zu", decoded.length() - fillLength, plaintextLen);
		//	fillLength of bytes is needed to guess last byte
		int dl = decoded.length() - fillLength;
		//	length of prefix string is shrinking with more
		//	characters that are decrypted(so (blockLen-1)-dl % blockLen
		//	which just mean amount of characters missing to fill block
		int fdl = (blockLen-1)-dl%blockLen;
		string test = mark;
		test.append(decoded.substr(dl, fdl));
		size_t off;
		string c1;
		do {
			c1 = oracle.encrypt(test);
			off = c1.find(encMark);
			tries++;
		} while(off == string::npos);
		plaintextLen = min(plaintextLen, c1.length() - off - encMark.length() - fdl);
		c1 = c1.substr(off+encMark.length()+dl/blockLen*blockLen, blockLen);
		char result = 0;
		for(const char& c : alphabet) {
			ostringstream input;
			string tryMe = decoded.substr(decoded.length()-fillLength) + c;
			for(unsigned int i = 0; i < blockLen; i++)
				input << mark << tryMe << 'X';
			string c2 = oracle.encrypt(input.str());
			off = c2.find(encMark);
			if(c1 == c2.substr(off + encMark.length(), blockLen)) {
				result = c;
				break;
			}
		}
		if(result == 0) {
			ret = false;
			break;
		}
		decoded.append(1, result);
	} while(plaintextLen > decoded.length() - 15 + 1);
	decoded = decoded.substr(fillLength);
	log("\n[>] Avg. %lf guesses per chunk", (float)(tries)/decoded.length());
	return ret;
}

size_t discoverBlockLen(encOracle& oracle, size_t expected,
						int testAmount, bool verbose)
{
	//	discover block size for oracle
	set<size_t> cryptLen;
	int testsCnt = 0;
	size_t blockLen;
	string test;
	do {
		int cnt = 0;
		while(cnt < testAmount) {
			test.append(1, 'A');
			string c1 = oracle.encrypt(test);
			cryptLen.insert(c1.length());
			cnt++;
		}
		testsCnt += cnt;
		int p = *cryptLen.begin();
		blockLen = *cryptLen.rbegin();
		for(const int& i: cryptLen) {
			if (i-p > 0 && (size_t)(i-p) < blockLen)
				blockLen = i-p;
			p = i;
		}
		log("[>] After %d tries blocklen guess: %zu", testsCnt, blockLen);
		if (verbose) {
			printf("Continue?[y/N]");
			fflush(stdout);
		}
	} while(blockLen != expected && verbose && getchar() == 'y');
	return blockLen;
}

