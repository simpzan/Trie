#ifndef TRIE_NODE_H
#define TRIE_NODE_H

#include <stdint.h>
#include <string>
#include <vector>
#include <map>
#include "TrieNodeInterface.h"

class TrieNode : public TrieNodeInterface {
 public:
  TrieNode() : _value(0) {}
  virtual ~TrieNode() {}

  virtual TrieNode *getChildNodeWithLabel(uint8_t ch) = 0;
  virtual void setChildNodeWithLabel(uint8_t ch, TrieNode* node) = 0;
  virtual void removeChildNodeWithLabel(uint8_t ch) = 0;
  virtual int childCount() = 0;

  virtual void clear() {  _value = 0;  }
  virtual TrieNode *createNode() = 0;

  virtual TrieValueType getValue() {  return _value;  }
  virtual void setValue(TrieValueType value) {  _value = value;  }

  virtual void traverseDFS(TrieNodeVisitorInterface &visitor) = 0;
  virtual void getStringsInSubtrie(const std::string &prefix, 
      std::map<std::string, TrieValueType>& entries) = 0;

 private:
  TrieValueType _value;
};

#endif
