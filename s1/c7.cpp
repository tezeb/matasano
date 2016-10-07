#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string>
#include "utils.h"
#include "aes128_ecb.h"

using namespace std;

int main() {
  string key = "YELLOW SUBMARINE";

  aes128_ecb a(key);
  a.init(false);
  a.append(b64dec(readFile("../s1/7.txt")));
  string ret = a.finalize();

  printf("Decrypted:\n%s\n", ret.substr(0, 256).c_str());

  return 0;
}
