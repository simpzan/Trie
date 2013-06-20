#ifndef PTRIE_H
#define PTRIE_H

#include <stdint.h>
#include <assert.h>
#include <queue>
#include <string>
#include <vector>
#include "TrieInterface.h"
#include "TrieNode.h"
#include "PTrieNode.h"
#include "TrieIteratorInterface.h"

class TrieBfsIterator : public TrieIteratorInterface {
 public:
  TrieBfsIterator(TrieInterface &trie);
  virtual ~TrieBfsIterator() {}

  virtual TrieNodeInterface *next();

 private:
  TrieInterface &_trie;
  std::queue<TrieNodeInterface *> _nodes;
};

class PTrie : public TrieInterface {
 public:
  PTrie() : _root(new PTrieNode), _node_count(1), _key_count(0) {}
  virtual ~PTrie() {}

  virtual void addEntry(const char *key, TrieValueType value);
  virtual void clear();

  // find the value associated with key.
  // return true if found, false otherwise.
  virtual bool findEntry(const char *key, TrieValueType &value);
  // find the first entry whose key is greater than or equal to `pattern`.
  virtual bool findEntryLowerBound(const char *pattern, 
      std::string *key, TrieValueType &value) {  assert(false);  }
  // find all key value pairs in this trie whose key is prefixed by `prefix`.
  virtual void prefixSearch(const char *prefix, 
      std::map<std::string, TrieValueType> &matches) {  assert(false);  }

  virtual void traverseBFS(TrieNodeVisitorInterface &visitor);
  virtual void traverseDFS(TrieNodeVisitorInterface &visitor);

  virtual uint32_t getNodeCount() const {  return _node_count;  }
  // return the count of key value pairs in this trie.
  virtual uint32_t getKeyCount() const {  return _key_count;  }

  virtual TrieNodeInterface *root() {  return _root;  }

  TrieBfsIterator BfsIterator() {  return TrieBfsIterator(*this);  }

  // collect all labels and generate link info.
  // must called before transform to LoudsMap.
  void collectLabels(std::vector<std::string> &labels);
  bool canAddEntry(const char *key, uint32_t value, int block_size);
  void assignNodeIds() {  _root->assignNodeIds();  }

  friend class TrieBfsIterator;

 private:
  

  PTrieNode *_root;
  uint32_t _node_count;
  uint32_t _key_count;
};


#endif
