#ifndef XOR_UTILS_H
#define XOR_UTILS_H

#include <vector>
#include <utility>
#include <string>

#include "utils.h"
#include "xorer.h"

namespace {

  using std::vector;
  using std::string;
  using std::pair;

  struct brute_result {
    brute_result(const string& dec, char key) : decrypted(dec), c(key) {}
    string decrypted;
    char c;
  };

  pair<vector<brute_result>, int> bruteXor_eng_char(const string& in) {
    xorer x;
    int max_sum = 0;
    vector<brute_result> ret;
    for(int c = 0; c < 256; c++) {
      x.setKey(c) << in;
      int sum = getEngScore(x.get());
      if(sum >= max_sum) {
        if(sum > max_sum) {
          max_sum = sum;
          ret.clear();
        }
        ret.push_back(brute_result(x.get(), c));
      }
      x.reset();
    }
    return pair<vector<brute_result>, int>(ret, max_sum);
  }
}

#endif  // XOR_UTILS_H
