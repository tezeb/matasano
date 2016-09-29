#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <string>
#include "utils.h"

using namespace std;

void loadAndDecrypt(const string &in, string& key) {
  /* Load the human readable error strings for libcrypto */
  ERR_load_crypto_strings();

  /* Load all digest and cipher algorithms */
  OpenSSL_add_all_algorithms();

  /* Load config file, and other important initialisation */
  OPENSSL_config(NULL);

  EVP_CIPHER_CTX *ctx;
  int len = in.length();
  int olen;

  unsigned char *out = new unsigned char[len+1];

  if(!(ctx = EVP_CIPHER_CTX_new())) {
     ERR_print_errors_fp(stderr);
     return;
  }

  if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL,
                             reinterpret_cast<const unsigned char*>(key.c_str()),
                             NULL)) {
     ERR_print_errors_fp(stderr);
     return;
  }

  if(1 != EVP_DecryptUpdate(ctx, out, &olen,
                            reinterpret_cast<const unsigned char*>(in.c_str()),
                            len)) {
     ERR_print_errors_fp(stderr);
     return;
  }

//  BIO_dump_fp(stdout, reinterpret_cast<const char*>(out), 128);

  int xlen;
  if(1 != EVP_DecryptFinal_ex(ctx, out+olen, &xlen)) {
     ERR_print_errors_fp(stderr);
     return;
  }
  olen += xlen;

  EVP_CIPHER_CTX_free(ctx);
  out[(256<olen?256:olen)] = '\0';

	printf("Decrypted:\n%s\n", out);
  printf("Enc: %d, Dec: %d\n", len, olen);

  free(out);

  /* Removes all digests and ciphers */
  EVP_cleanup();

  /* if you omit the next, a small leak may be left when you make use of the BIO (low level API) for e.g.
   * base64 transformations */
  CRYPTO_cleanup_all_ex_data();

  /* Remove error strings */
  ERR_free_strings();
}

int main() {
  string key = "YELLOW SUBMARINE";

  loadAndDecrypt(b64dec(readFile("7.txt")), key);

  return 0;
}
