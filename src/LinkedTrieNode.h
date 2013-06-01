#ifndef LINKEDTRIENODE_H
#define LINKEDTRIENODE_H

#include <map>
#include "TrieNode.h"

class LinkedTrieNode: public TrieNode {
 public:
  LinkedTrieNode() {}
  virtual ~LinkedTrieNode();

  virtual TrieNode *getChildNodeWithLabel(uint8_t ch);
  virtual void setChildNodeWithLabel(uint8_t ch, TrieNode *node);
  virtual int childCount() {  return _children.size();  }

  virtual void clear();
  virtual LinkedTrieNode *createNode() {
    LinkedTrieNode *node = new LinkedTrieNode();
    return node;
  }

  std::map<uint8_t, TrieNode *> &children() {  return _children;  }

 private:
  std::map<uint8_t, TrieNode*> _children;
};


#endif

