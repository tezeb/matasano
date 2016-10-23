#include "utils.h"
#include <map>
#include <algorithm>
#include <fstream>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;

namespace {

  const char b64enc_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K',
    'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',
    'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g',
    'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
    's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2',
    '3', '4', '5', '6', '7', '8', '9', '+', '/'};

}

const char* InvalidPadding::what() const noexcept {
	return "Invalid Padding!";
}

string b64enc(const string& in) {
  string ret;
  int buf = 0;
  int n = 0;
  for(size_t i = 0; i < in.length(); i++) {
    buf |= (unsigned char)in[i];
    n++;
    if(n == 3) {
      for(int m = 18; m >= 0; m-=6) {
          ret.append(1, b64enc_table[buf>>m & 63]);
      }
      n = 0;
      buf = 0;
    }
    buf <<= 8;
  }
  if(n > 0) {
    for(int m = (n==1?10:18); m > 0; m-=6)
        ret.append(1, b64enc_table[buf>>m & 63]);
    ret.append(3-n, '=');
  }
  return ret;
}

char b64Index(char in) {
  if(in >= 'A' && in <= 'Z')
      return in-'A';
  if(in >= 'a' && in <= 'z')
      return in-'a'+26;
  if(in >= '0' && in <= '9')
      return in-'0'+52;
  if(in == '+')
      return 62;
  if(in == '/')
      return 63;
  return -1;
}

string b64dec(const string& in) {
  int buf = 0;
  string ret;
  //  TODO: add handling of unpadded b64
  for(size_t i = 0; i < in.length(); i+=4) {
    size_t j = i;
    buf = b64Index(in[j++]);
    int cnt = 4;
    while(j < i+4 && in[j] != '=') {
      buf <<= 6;
      buf |= b64Index(in[j]);
      ret.append(1, buf>>cnt & 0xFF);
      cnt-=2;
      j++;
    }
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
  for(const unsigned char& c : a) {
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

int editDist(std::string& a, std::string& b) {
  if(a.length() != b.length()) 
      return -1;
  auto ai = a.begin();
  auto bi = b.begin();
  int ret = 0;
  while(ai != a.end() && bi != b.end()) {
    ret+=editDist(*ai, *bi);
    ai++;
    bi++;
  }
  return ret;
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

string readFile(string filename) {
  ifstream file(filename);
  string ret;
  string tmp;
  while(!getline(file, tmp).eof()) {
    ret.append(tmp);
  }
  file.close();
  return ret;
}

string readBinFile(string filename) {
  ifstream file(filename);
  string ret;
  string tmp;
  char buf[1024];
  while(!file.read(buf, 1024).eof()) {
    ret.append(buf);
  }
  if(file.fail()) {
    ret.append(buf, file.gcount());
  }
  file.close();
  return ret;
}

string& pad(string& in, char block_size)
{
	char padding = block_size - in.length()%block_size;
	in.append(padding, padding);
	return in;
}

std::string& unpad(std::string& in)
{
	int val = in[in.length()-1];
	size_t i = in.length()-val;
	if(i > in.length() || val < 1)
		throw InvalidPadding();
	while(i < in.length()) {
		if(in[i] != val)
			throw InvalidPadding();
		i++;
	}
	in.erase(in.length()-val);
	return in;
}

string createRandomString(size_t length) {
	string ret;
	while(length--) {
		ret.append(1, rand()%255);
	}
	return ret;
}

string url_encode(const string &value) {
    ostringstream escaped;
    escaped.fill('0');
    escaped << hex;

    for (string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
        string::value_type c = (*i);

        // Keep alphanumeric and other accepted characters intact
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
        }
		// Any other characters are percent-encoded
		else {
			escaped << uppercase;
			escaped << '%' << setw(2) << int((unsigned char) c);
			escaped << nouppercase;
		}
    }

    return escaped.str();
}

string url_decode(const string &value) {
    ostringstream result;

    string::const_iterator i = value.begin(), n = value.end();
	while(i != n) {
        string::value_type c = (*i++);

		if (c == '%') {
			string nbr(1, *i++);
			nbr.append(1, *i++);
			result << static_cast<char>(stoi(nbr, nullptr, 16));
		}
		else {
			result << c;
		}
    }

    return result.str();
}

string hexNonPrint(const string& in) {
	ostringstream out;
	out << hex << setfill('0');
	for(const char& i: in) {
		if(isprint(i))
			out << i;
		else
			out << "\\x" << setw(2) << +static_cast<unsigned char>(i);
	}
	return out.str();
}

map<string, string> splitUrlValues(const string& cookie) {
	map<string, string> ret;
	size_t pos = 0;
	size_t prev_pos = 0;
	while((pos = cookie.find_first_of(";=", prev_pos)) != string::npos) {
		string key = cookie.substr(prev_pos, pos-prev_pos);
		string value;
		prev_pos = pos + 1;
		if(cookie[pos] == '=') {
			if ((pos = cookie.find_first_of(';', prev_pos)) == string::npos) {
				pos = cookie.length();
			}
			value = cookie.substr(prev_pos, pos-prev_pos);
			prev_pos = pos + 1;
		}
		ret.insert(pair<string, string>(key, value));
	}
	if(prev_pos < cookie.length())
		ret.insert(pair<string, string>(cookie.substr(prev_pos, pos-prev_pos), string()));
	return ret;
}
