#ifndef TRIE_H
#define TRIE_H

#include <utility>
#include <string>
#include <stdint.h>
#include <vector>
#include <assert.h>
#include <cstring>
#include <map>
#include "TrieNode.h"

class Trie {
 public:
  Trie(TrieNode *node) : _root(node), _node_count(1), _value_count(0) {}
  virtual ~Trie() {  if (_root)  delete _root;  }

  // add an entry with the key/value 
  void addEntry(const char *key, TrieValueT value);
  void undoAdd();
  TrieValueT getEntry(const char *key);
  // prefix search the pattern 
  bool prefixSearch(const char *pattern, 
      std::map<std::string, TrieValueT>& results);
  void clear();

  uint64_t node_count() {  return _node_count;  }
  uint32_t nodeCountAfterInsert(const char *key);
  uint64_t value_count() {  return _value_count;  }

  const std::string &get_last_key() const {  return _last_key;  }
  void traversePreorderly(TrieVisitorInterface &visitor) {
    return _root->traversePreorderly(visitor);
  }

 private:
  bool _followKey(const char *key, std::vector<TrieNode *> &nodes);
  void createNodes(const char *key, TrieNode *node, 
      std::vector<TrieNode *> &nodes);

  TrieNode *_root;
  uint64_t _node_count;
  uint64_t _value_count;
  std::string _last_key;
};

#endif
