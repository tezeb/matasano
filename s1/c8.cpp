#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <cstdio>
#include <fstream>
#include <string>
#include "utils.h"

using namespace std;

int main() {
  string filename = "8.txt";
  ifstream file(filename);
  string tmp1;
  int max_cnt = 0;
  vector<string> res;
  while(!getline(file, tmp1).eof()) {
    int cnt = 1;
    string tmp = unhexlify(tmp1);
    for(size_t i = 0; i < tmp.length(); i+=16) {
      for(size_t j = i+16; j < tmp.length(); j+=16) {
        if(tmp.substr(i, 16) == tmp.substr(j, 16)) {
          cnt++;
          break;
        }
      }
    }
    if(cnt >= max_cnt) {
      if(cnt > max_cnt) {
        max_cnt = cnt;
        res.clear();
      }
      res.push_back(tmp1);
    }
  }
  file.close();

  printf("Max count: %d\n", max_cnt);
  for(auto i = res.begin(); i != res.end(); i++) {
    printf("%s\n", i->c_str());
    /*
    for(size_t j = 0; j < i->length(); j+=16) {
      printf("%s\n", i->substr(j, 16).c_str());
    }
    */
  }
	return 0;
}
