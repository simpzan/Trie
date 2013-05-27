#ifndef TRIE_H
#define TRIE_H

#include <utility>
#include <string>
#include <stdint.h>
#include <vector>
#include <assert.h>
#include <cstring>
#include "TrieNode.h"

class Trie {
 public:
  Trie(TrieNode *node) : _root(node), _node_count(1), _value_count(0) {}
  virtual ~Trie() {  if (_root)  delete _root;  }

  // add an entry with the key/value 
  void addEntry(const char *key, uint64_t value);
  uint64_t getEntry(const char *key);

  // prefix search the pattern 
  bool prefixSearch(const char *pattern, std::vector<EntryT>& results);
  uint32_t sizeInByte() {  return _root->sizeInByte();  }
  uint32_t sizeInMegaByte() {	return _root->sizeInMegaByte(); }

  TrieNode *root() {  return _root;  }
  uint64_t node_count() {  return _node_count;  }
  uint64_t value_count() {  return _value_count;  }
  void clear();

 private:
  TrieNode *_root;
  uint64_t _node_count;
  uint64_t _value_count;
};

#endif
