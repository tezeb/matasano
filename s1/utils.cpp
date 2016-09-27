#include "utils.h"
#include <map>
#include <algorithm>

using namespace std;

namespace {

  const char b64enc[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
    'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
    'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g',
    'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
    's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2',
    '3', '4', '5', '6', '7', '8', '9', '+', '/'};

}

//	TODO: b64dec missing

//	takes ptr to string and amount of bytes to convert
//	len have to be even
string hex2b64(const string& in, size_t len) {
  //	assert len % 2 == 0
  string ret;
  int buf = 0;
  int n = 0;
  for(size_t i = 0; i < len; i+=2) {
    buf |= h2c(in[i], in[i+1]);
    n++;
    if(n == 3) {
      for(int m = 18; m >= 0; m-=6)
          ret.append(1, b64enc[buf>>m & 63]);
      n = 0;
      buf = 0;
    }
    buf <<= 8;
  }
  if(n > 0) {
    for(int m = (n==1?10:18); m > 0; m-=6)
        ret.append(1, b64enc[buf>>m & 63]);
    ret.append(3-n, '=');
  }
  return ret;
}

int getHexValue(const char a) {
  if(a >= '0' && a <= '9')
      return a-'0';
  else if((a >= 'a' && a <= 'f'))
      return a-'a'+10;
  else if((a >= 'A' && a <= 'F'))
      return a-'A'+10;
  return 0;
}

char h2c(const char a, const char b) {
  int r = 16*getHexValue(a);
  r += getHexValue(b);
  return r;
}

char c2h(char a) {
  if(a >= 0 && a < 10)
      return a+'0';
  else if(a > 9 && a < 16)
      return a+'a'-10;
  return 'X';
}

std::string hexlify(const std::string& a) {
  std::string ret(a.length()*2, 0);
  int i = 0;
  for(const char& c : a) {
      ret[i] = c2h(c/16);
 //     printf("%c[%x] %c\t", c, c, ret[i]);
      i++;
      ret[i] = c2h(c%16);
//      printf("%c\n", ret[i]);
      i++;
  }
  return ret;
}

std::string unhexlify(const std::string& a) {
  std::string ret(a.length()/2, 0);
  for(size_t i = 0, n=0; i+1 < a.length(); i+=2, n++) {
      ret[n] = h2c(a[i], a[i+1]);
  }
  return ret;
}

int editDist(unsigned char a, unsigned char b) {
  char c = a ^ b;
  int ret = 0;
  while(c) {
    ret++;
    c &= c-1;
  }
  return ret;
}

int editDist(const char* a, const char* b) {
  int ret = 0;
  while(*a && *b) {
    ret+=editDist(*a, *b);
    a++;
    b++;
  }
  if(*a != *b)
      return -1;
  return ret;
}

int editDist(std::string& a, std::string& b) {
  return editDist(a.c_str(), b.c_str());
}

vector<pair<char,int>> frequencer(const string& a) {
  map<char, int> ret;
  for(size_t i = 0; i < a.length(); i++) {
    if(ret.find(a[i]) == ret.end())
        ret[a[i]] = 0;
    ret[a[i]]++;
  }
  vector<pair<char, int>> freq(ret.begin(), ret.end());
  sort(freq.begin(), freq.end(), 
       [](pair<char, int> p1, pair<char, int> p2)
       ->bool{return p1.second > p2.second;});
  return freq;
}
//  adjusted as in: http://www.macfreek.nl/memory/Letter_Distribution
map<char, int> letter_freq_centroid = { {' ', 18}, {'e', 10}, {'t', 7}, {'a', 6}, 
    {'o', 6}, {'i', 5}, {'n', 5}, {'s', 5}, {'r', 5}, {'h', 4}, {'d', 4},
    {'l', 3}, {'u', 2}, {'c', 2}, {'m', 2}, {'f', 2}, {'y', 2}, {'w', 2},
    {'g', 2}, {'p', 1}, {'b', 1}, {'v', 1}, {'k', 0}, {'x', 0}, {'q', 0},
    {'j', 0}, {'z', 0} };
int getEngScore(const string& in) {
  auto v = frequencer(in);
  int sum = 0;
  //  english matching done according to:
  //  http://www.sucati.org/yayin/LBTS_LI.pdf
  for(auto i = v.begin(); i != v.end(); i++) {
    if(letter_freq_centroid.find(i->first) != letter_freq_centroid.end())
        sum+=i->second*letter_freq_centroid[i->first];
  }
  return sum;
}
