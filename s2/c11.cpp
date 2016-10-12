#include <unistd.h>
#include <cstdio>
#include <fstream>
#include <string>
#include "utils.h"
#include "aes128_ecb.h"
#include "aes128_cbc.h"

using namespace std;

class allMightyOracle {
	int m_prev;
	int m_ecbGCorrect;
	int m_cbcGCorrect;
	int m_total;
	public:
	allMightyOracle() : m_prev(-1), m_ecbGCorrect(0), m_cbcGCorrect(0), m_total(0) {}
	string encryptRandomly(const string& input) {
		//	prefix
		string data = createRandomString(rand()%5 + 5);
		data += input;
		//	suffix
		data += createRandomString(rand()%5 + 5);
		//	padding
		data = pad(data, 16);
		string key = createRandomString(16);
		string iv = createRandomString(16);
		aes128_ecb ecb(key);
		aes128_cbc cbc(key);
		m_prev = rand();
		m_total++;
		if(m_prev%2 == 0) {
			return ecb.encrypt(data);
		}
		return cbc.encrypt(data, iv);
	}
	void guessECB() {
		if(m_prev != -1 && m_prev%2 == 0)
			m_ecbGCorrect++;
		m_prev = -1;
	}
	void guessCBC() {
		if(m_prev != -1 && m_prev%2 != 0)
			m_cbcGCorrect++;
		m_prev = -1;
	}
	void showStats() {
		log("Tested:\t%d\nECB:\t%d\nCBC:\t%d\n%s", m_total, m_ecbGCorrect,
			   m_cbcGCorrect, (m_total == m_cbcGCorrect + m_ecbGCorrect)?"All guessed OK":"FAILED!!!");
	}
};

bool isECB(const string& cryptxt) {
	return (countRepetitions(cryptxt) > 1);
}

int main() {
	srand(time(NULL));
	allMightyOracle oracle;
	string data(48, 'A');
	for(int i = 0; i < 256; i++) {
		string cryptxt = oracle.encryptRandomly(data);
		if(isECB(cryptxt))
			oracle.guessECB();
		else
			oracle.guessCBC();
	}
	oracle.showStats();
	return 0;
}
