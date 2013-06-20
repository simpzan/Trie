#ifndef TRIE_INTERFACE_H
#define TRIE_INTERFACE_H

#include <stdint.h>
#include <cassert>
#include <string>
#include <map>
#include "TrieNodeInterface.h"
#include "TrieIteratorInterface.h"

class TrieInterface {
  public:
  TrieInterface() {}
  virtual ~TrieInterface() {}
  
  virtual void addEntry(const char *key, TrieValueType value) = 0;
  virtual void clear() = 0;

  // find the value associated with key.
  // return true if found, false otherwise.
  virtual bool findEntry(const char *key, TrieValueType &value) = 0;
  // find the first entry whose key is greater than or equal to `pattern`.
  virtual bool findEntryLowerBound(const char *pattern, 
      std::string *key, TrieValueType &value) {  assert(false);  }
  // find all key value pairs in this trie whose key is prefixed by `prefix`.
  virtual void prefixSearch(const char *prefix, 
      std::map<std::string, TrieValueType> &matches) {  assert(false);  }

  virtual void traverseBFS(TrieNodeVisitorInterface &visitor) {  assert(false);  }
  virtual void traverseDFS(TrieNodeVisitorInterface &visitor) {  assert(false);  }

  //virtual TrieIteratorInterface BfsBegin() {  assert(false);  }
  //virtual TrieIteratorInterface BfsEnd() {  assert(false);  }

  virtual uint32_t getNodeCount() const = 0;
  // return the count of key value pairs in this trie.
  virtual uint32_t getKeyCount() const = 0;

  virtual TrieNodeInterface *root() = 0;
};

#endif
