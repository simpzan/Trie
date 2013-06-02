#ifndef LINKEDTRIENODE_H
#define LINKEDTRIENODE_H

#include <stdint.h>
#include <boost/ptr_container/ptr_map.hpp>
#include <map>
#include "TrieNode.h"

using boost::ptr_map;
typedef ptr_map<uint8_t, TrieNode>::const_iterator ChildrenMapIterator;

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

  void getLabels(std::vector<uint8_t> &labels) {
    for (ChildrenMapIterator itr = _children.begin(), end = _children.end();
        itr != end;
        ++itr) {
      labels.push_back(itr->first);
    }
  }
 private:
  boost::ptr_map<uint8_t, TrieNode> _children;
};

#endif

