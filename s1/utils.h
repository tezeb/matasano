#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>

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

#endif // _UTILS_H_
