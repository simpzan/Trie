
#ifndef STRING_POOL_H
#define STRING_POOL_H

#include <stdint.h>
#include <iostream>
#include <map>

#include "Vector.h"
#include "BitVectorBuilder.h"
#include "Range.h"
#include "LoudsTrie.h"
#include "LoudsTrieBuilder.h"
#include "STrie.h"

class StringPool : public ILoudsTrieBuilder {
public:
  StringPool() {}
  virtual ~StringPool() {}

  uint32_t findOrInsert(const char *key);
  void build();
  void serialize(std::ostream &os);
  std::vector<uint32_t>& newIds() { return _newIds; }

  virtual sdsl::bit_vector &louds()  { return _louds.underlyingBV(); }
  virtual Vector<uint8_t> &labels() {  return _labels;  }

private:
  bool isTerminalNode(Range &range) {
    return _keys[range.left].size() == range.depth;
  }
  void getSubranges(Range &range, std::vector<Range> &subranges);
  void generateData();
  void display() {
    cout << louds() << endl;
    labels().display(cout);
  }

private:
  typedef std::map<std::string, uint32_t> StringIntMap;
  StringIntMap _keyMap;
  std::vector<string> _keys;
  std::vector<uint32_t> _oldIds;

  BitVectorBuilder _louds;
  Vector<uint8_t> _labels;
  std::vector<uint32_t> _newIds;
};

#endif

