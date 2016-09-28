#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <unistd.h>
#include <vector>
#include <string>
#include <utility>

//  hex to base64 encode
std::string hex2b64(const std::string& in, size_t len);

std::string b64dec(const std::string& in);

int getHexValue(const char a);

//  converts number 0xab stored as 2 chars to one it's numerical representation
char h2c(const char a, const char b);

char c2h(char a);

//  change string to it's hex representation(each character is represented as to hex values)
std::string hexlify(const std::string& a);

//  read hex represented string each character pair is changed into coresponding numerical value
std::string unhexlify(const std::string& a);

int editDist(unsigned char a, unsigned char b);

int editDist(const char* a, const char* b);

int editDist(std::string& a, std::string& b);

//  function returns vector of frequencies for all letters in string
std::vector<std::pair<char,int>> frequencer(const std::string& a);

//  function scores english text
int getEngScore(const std::string& in);

std::string readFile(std::string filename);

#endif // _UTILS_H_
