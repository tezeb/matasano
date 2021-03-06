#ifndef _UTILS_H_
#define _UTILS_H_

#include <cstdio>

#include <vector>
#include <string>
#include <utility>
#include <exception>
#include <map>

#include <unistd.h>

#define dbg(...) do {fprintf(stderr, "%20.20s:%03d\t", __func__, __LINE__); fprintf(stderr, __VA_ARGS__); putc('\n', stderr);} while(0)
#define log(...) do {fprintf(stderr, __VA_ARGS__); putc('\n', stdout);} while(0)

struct InvalidPadding : std::exception {
	virtual const char* what() const noexcept override;
};

std::string b64enc(const std::string& in);

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

std::string readBinFile(std::string filename);

std::string& pad(std::string& in, char block_size);

std::string& unpad(std::string& in);

std::string createRandomString(size_t length);

std::string url_encode(const std::string &value);

std::string url_decode(const std::string &value);

std::string hexNonPrint(const std::string& in);

std::map<std::string, std::string> splitUrlValues(const std::string& cookie);

#endif // _UTILS_H_
