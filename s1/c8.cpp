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
    string tmp = unhexlify(tmp1);
    int cnt = countRepetitions(tmp);
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
