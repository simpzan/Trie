#ifndef SUCCINCT_MAP_H
#define SUCCINCT_MAP_H

#include <iostream>
#include "TrieInterface.h"

class SuccinctMap {
 public:
  SuccinctMap() {}
  virtual ~SuccinctMap() {}

  virtual bool build(TrieInterface &trie) = 0;
  virtual bool load(std::istream &is) = 0;
  virtual bool serialize(std::ostream &os) = 0;
  virtual void clear() = 0;

  // find the value associated with key.
  // return true if found, false otherwise.
  virtual bool findEntry(const char *key, TrieValueType &value) = 0;
  // find the first entry whose key is greater than or equal to `pattern`.
  virtual bool findEntryLowerBound(const char *pattern, 
      std::string *key, TrieValueType &value) {  assert(false);  }
};

#endif
