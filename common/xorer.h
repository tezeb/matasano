#ifndef _XORER_H_
#define _XORER_H_

#include <string>

class xorer {
  std::string m_key;
  std::string m_buffer;
  std::string::iterator m_iter;
  void increment() {
      m_iter++;
      if(m_iter == m_key.end())
          m_iter = m_key.begin();
  }
public:
  xorer(std::string key="") : m_key(key) {
    m_iter = m_key.begin();
  }
  xorer(char key) : m_key(1, key) {
    m_iter = m_key.begin();
  }
  xorer& operator<<(const std::string& in) {
    for(auto i = in.cbegin(); i != in.cend(); i++) {
      m_buffer.append(1, *i^*m_iter);
      increment();
    }
    return *this;
  }
  xorer& append(const char* in, size_t size) {
    while(size--) {
      m_buffer.append(1, *in^*m_iter);
      in++;
      increment();
    }
    return *this;
  }
  std::string get() {
    return m_buffer;
  }
  void reset() {
    m_buffer.clear();
    m_iter = m_key.begin();
  }
  void reset(std::string key) {
	  m_buffer.clear();
	  m_key = key;
	  m_iter = m_key.begin();
  }
  xorer& setKey(std::string key) {
    m_key = key;
    m_iter = m_key.begin();
    return *this;
  }
  xorer& setKey(char key) {
    return setKey(std::string(1, key));
  }
};

#endif // _XORER_H_
