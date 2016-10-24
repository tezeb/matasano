#include <string>
#include "utils.h"
#include "aes128_ctr.h"

using namespace std;

int main() {
	string secret(b64dec("L77na/nrFsKvynd6HzOoG7GHTLXsTVu9qvY/2syLXzhPweyyMTJULu/6/kXX0KSvoOLSFQ=="));
	string key("YELLOW SUBMARINE");
	aes128_ctr s(key);
	log("%s", hexNonPrint(s.decrypt(secret)).c_str());
	s.init();
	for(char& c : secret) {
		s.append(string(1, c));
	}
	log("%s", hexNonPrint(s.finalize()).c_str());
	return 0;
}
