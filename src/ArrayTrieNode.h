#ifndef ARRAYTRIENODE_H
#define ARRAYTRIENODE_H

#include <stdint.h>
#include <cassert>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <typeinfo>
#include <bitset>
#include "TrieNode.h"

#define NODESIZE 256 

class ArrayTrieNode: public TrieNode {
 public:
  ArrayTrieNode();
  virtual ~ArrayTrieNode();

  // return the child with the char in this node.
  virtual TrieNode *getChildNodeWithLabel(uint8_t ch) {
    return _children[ch];
  }
  // set the child with the char in this node.
  virtual void setChildNodeWithLabel(uint8_t ch, TrieNode *node) {
    _children[ch] = node;
  }

  void setValue(uint64_t value) {  _value = value;  }
  uint64_t getValue() {  return _value;  }

  virtual ArrayTrieNode *createNode() {
    ArrayTrieNode *node = new ArrayTrieNode;
    return node;
  }

  virtual uint32_t sizeOfThisNode() {
    uint32_t size = sizeof(_value) + sizeof(_children);
    return size;
  }	

  virtual void clear();

 protected:
  uint64_t _value;
  TrieNode *_children[NODESIZE]; 	// children links
};

inline ArrayTrieNode::ArrayTrieNode() : _value(0) {
  memset(_children, 0, sizeof(_children));
}

inline ArrayTrieNode::~ArrayTrieNode() {
  this->clear();
}

inline void ArrayTrieNode::clear() {
  for (int i = 0; i < NODESIZE; ++i) {
    if (_children[i]) { 
      delete _children[i];
      _children[i] = NULL;
    }
  }
  _value = 0;
}

#endif
