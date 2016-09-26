#ifndef _B64_h_
#define _B64_h_

class b64 {
	string m_buff;
	int buf;
	static const char b64enc[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};

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

	public:
	b64() {}
	b64& appendHex(const char* in, size_t len) {

	}

	b64& append(const char* in) {
	}

	b64& operator<<(const string& in) {
	}
	string operator() {
	}
};

#endif //	 _B64_h_
