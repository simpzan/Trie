#ifndef FRONT_CODER_H
#define FRONT_CODER_H

#include <vector>
#include <string>
#include "Vector.h"
#include "ConstVector.h"

class FrontCoderBuilder {
  public:
  FrontCoderBuilder() {}
  ~FrontCoderBuilder() {}

  void add(const std::string &key);
  uint64_t save(std::ostream &os);
  void clear();

  const uint8_t *firstKey() const;
  uint64_t count() const {  return _LCPs.count();  }
  uint64_t size() const;

 private:
  Vector<uint8_t> _LCPs;
  Vector<uint8_t> _suffix_lengths;
  Vector<uint8_t> _suffixes;
  std::string _lastKey;
};

inline uint64_t FrontCoderBuilder::size() const {
  return _LCPs.size() + _suffix_lengths.size() + _suffixes.size();
}

class FrontCoder {
 public:
  FrontCoder() {}
  ~FrontCoder() {}

  bool find(const std::string &key, uint32_t &rank);
  bool load(std::istream &is);
  uint64_t mmap(const uint8_t *address);
  void clear();
  const uint8_t *firstKey() const;
  uint64_t count() const {  return _LCPs.count();  }
  void display() const;
  
  private:
  ConstVector<uint8_t> _LCPs;
  ConstVector<uint8_t> _suffix_lengths;
  ConstVector<uint8_t> _suffixes;
};

inline void FrontCoder::display() const {
  std::ostream &os = std::cout;
  int count = this->count();
  std::string lastKey;
  uint64_t offset = 0;
  for (int i = 0; i < count; ++i) {
    std::string key(lastKey.c_str(), _LCPs[i]);
    key += (char *)_suffixes.data() + offset;
    os << i << ":" << key << "\t";
    offset += _suffix_lengths[i];
    lastKey = key;
  }
  os << std::endl;
}

inline void FrontCoder::clear() {
  _LCPs.clear();
  _suffix_lengths.clear();
  _suffixes.clear();
}

inline void FrontCoderBuilder::clear() {
  _LCPs.clear();
  _suffix_lengths.clear();
  _suffixes.clear();
  _lastKey.clear();
}

inline bool FrontCoder::load(std::istream &is) {
  _LCPs.read(is);
  _suffix_lengths.read(is);
  _suffixes.read(is);
  return true;
}

inline uint64_t FrontCoderBuilder::save(std::ostream &os) {
  uint64_t offset = os.tellp();
  _LCPs.write(os);
  _suffix_lengths.write(os);
  _suffixes.write(os);
  return offset;
}

inline uint64_t FrontCoder::mmap(const uint8_t *address) {
  const uint8_t *tmp_address = address;
  uint32_t consumed_size = _LCPs.mmap(tmp_address);
  tmp_address += consumed_size;

  consumed_size = _suffix_lengths.mmap(tmp_address);
  tmp_address += consumed_size;

  consumed_size = _suffixes.mmap(tmp_address);
}

inline const uint8_t *FrontCoder::firstKey() const {
  if (count() == 0)  return NULL;
  return _suffixes.data();
}

#endif
