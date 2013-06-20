#ifndef TRIE_H
#define TRIE_H

#include <utility>
#include <string>
#include <stdint.h>
#include <vector>
#include <assert.h>
#include <cstring>
#include <map>
#include "TrieInterface.h"
#include "TrieNode.h"
#include "LinkedTrieNode.h"
#include "PTrie.h"
#include "LoudsTrie.h"

class Trie : public TrieInterface {
 public:
  Trie(TrieNode *node) : _root(node), _node_count(1), _value_count(0) {}
  virtual ~Trie() {  if (_root)  delete _root;  }

  virtual void addEntry(const char *key, TrieValueType value);
  virtual void clear();

  virtual bool findEntry(const char *key, TrieValueType &value);
  virtual uint32_t getNodeCount() const {  return _node_count;  }
  virtual uint32_t getKeyCount() const {  return _value_count;  }

  virtual TrieNodeInterface *root() {  return _root;  }
  virtual void traverseDFS(TrieNodeVisitorInterface &visitor) {
    _root->traverseDFS(visitor);  
  }

  TrieNode *addKey(const char *key);
  void computePrefix(TrieNode *node, std::string &label);
  
 private:
  bool _followKey(const char *key, std::vector<TrieNode *> &nodes);
  void _createNodes(const char *key, TrieNode *node, 
      std::vector<TrieNode *> &nodes);

  TrieNode *_root;
  uint64_t _node_count;
  uint64_t _value_count;
};

inline void Trie::clear() {
  _root->clear();
  _node_count = 1;
  _value_count = 0;
}

inline void Trie::addEntry(const char *key, TrieValueType value) {
  TrieNode *newNode = addKey(key);
  newNode->setValue(value);
  ++_value_count;
}

class LinkedTrie :public Trie {
  public:
	LinkedTrie() : Trie(new LinkedTrieNode) {}
	virtual ~LinkedTrie() {}
};

#endif
