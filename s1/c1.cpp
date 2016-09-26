#include <string>

using namespace std;

const char b64enc[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

//	TODO: b64dec missing

namespace {

	int getHexValue(const char a) {
		if(a >= '0' && a <= '9')
				return a-'0';
		else if((a >= 'a' && a <= 'f'))
				return a-'a'+10;
		else if((a >= 'A' && a <= 'F'))
				return a-'A'+10;
		return 0;
	}

	int hexToInt(const char* in) {
		int r = 16*getHexValue(*in);
		r += getHexValue(*(in+1));
		return r;
	}

	inline int appendFromBuff(int buff, int byte) {
    buff = buff;
    byte = byte;
    return 0;
	}

}

//	takes ptr to string and amount of bytes to convert
//	len have to be even
string hex2b64(const char* in, size_t len) {
	//	assert len % 2 == 0
	string ret;
	int buf;
	int n = 0;
	for(size_t i = 0; i < len; i+=2) {
		buf |= hexToInt(in+i);
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
	printf("%s\n", ret.c_str());
	return ret;
}

int main() {
	char t1[]="49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
	/* test padding
	hex2b64(t1, 8);
	hex2b64(t1, 6);
	hex2b64(t1, 4);
	hex2b64(t1, 2);
	*/
	hex2b64(t1, sizeof(t1)/sizeof(char)-1);
	hex2b64(t1, sizeof(t1)/sizeof(char)-3);
	hex2b64(t1, sizeof(t1)/sizeof(char)-5);
	hex2b64(t1, sizeof(t1)/sizeof(char)-7);
}
