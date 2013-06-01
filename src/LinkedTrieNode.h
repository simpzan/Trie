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
  virtual void removeChildNodeWithLabel(uint8_t ch);
  virtual int childCount() {  return _children.size();  }

  virtual void clear();
  virtual LinkedTrieNode *createNode() {
    LinkedTrieNode *node = new LinkedTrieNode();
    return node;
  }

  virtual void traversePreorderly(TrieVisitorInterface &visitor);
  virtual void getStringsInSubtrie(const std::string &prefix, 
      std::map<std::string, TrieValueT> &entries);

  std::map<uint8_t, TrieNode *> &children() {  return _children;  }

 private:
  std::map<uint8_t, TrieNode*> _children;
};


#endif

