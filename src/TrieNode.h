#ifndef TRIE_NODE_H
#define TRIE_NODE_H

#include <stdint.h>
#include <string>
#include <vector>
#include <map>

typedef uint64_t TrieValueT;

class TrieNode;
class TrieVisitorInterface {
 public:
  TrieVisitorInterface() {}
  virtual ~TrieVisitorInterface() {}

  virtual bool visitNode(TrieNode *node) = 0;
};

class TrieNode {
 public:
  TrieNode() : _value(0) {}
  virtual ~TrieNode() {}

  virtual TrieNode *getChildNodeWithLabel(uint8_t ch) = 0;
  virtual void setChildNodeWithLabel(uint8_t ch, TrieNode* node) = 0;
  virtual int childCount() = 0;

  virtual void clear() {  _value = 0;  };
  virtual TrieNode *createNode() = 0;

  TrieValueT getValue() {  return _value;  };
  void setValue(TrieValueT value) {  _value = value;  }

  void traversePreorderly(TrieVisitorInterface &visitor);
  void getStringsInSubtrie(const std::string &prefix, 
      std::map<std::string, TrieValueT>& entries);

 private:
  TrieValueT _value;
};

#endif
