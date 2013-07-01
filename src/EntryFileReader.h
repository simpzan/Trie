#ifndef ENTRYFILEREADER_H
#define ENTRYFILEREADER_H

#include <fstream>
#include <cstring>
#include <stdint.h>

#define KEY_SIZE_MAX 256

class EntryFileReader {
 public:
  EntryFileReader(const std::string &fname) {
    _inputStream.open(fname.c_str());
  }
  ~EntryFileReader() {}

  bool nextEntry(std::string &key, uint64_t &value) {
    value = 0;
    //value = _inputStream.tellg();
    //++value;
    key = "";

    char buf[KEY_SIZE_MAX];
    _inputStream.getline(buf, KEY_SIZE_MAX);
    if (!_inputStream.good())  return false;
     
    key = buf;
    value = strlen(buf);
    return strlen(buf);
  }

 private:
  std::ifstream _inputStream;
};

#endif
