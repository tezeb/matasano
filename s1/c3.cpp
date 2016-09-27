#include <cstdio>
#include <string>
#include "xor_utils.h"

using namespace std;

void run(const string& t1) {
  auto ret = bruteXor_eng_char(unhexlify(t1)).first;
  for(auto i = ret.begin(); i != ret.end(); i++) {
    printf("%s\n", i->decrypted.c_str());
  }
}

int main() {
	string t1="1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736";
	run(t1);
	return 0;
}
