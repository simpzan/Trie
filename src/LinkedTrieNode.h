#ifndef LINKEDTRIENODE_H
#define LINKEDTRIENODE_H

#include <stdint.h>
#include <boost/ptr_container/ptr_map.hpp>
#include "TrieNode.h"

using boost::ptr_map;
typedef ptr_map<uint8_t, TrieNode>::const_iterator ChildrenMapIterator;

class LinkedTrieNode: public TrieNode {
 public:
  //LinkedTrieNode() : _parent(NULL) {}
  virtual ~LinkedTrieNode();

  virtual TrieNodeInterface *getChildWithCharLabel(uint8_t ch) {
    return getChildNodeWithLabel(ch);
  }
  virtual TrieNode *getChildNodeWithLabel(uint8_t ch);
  virtual void setChildNodeWithLabel(uint8_t ch, TrieNode *node);
  virtual void removeChildNodeWithLabel(uint8_t ch);
  virtual int childCount() {  return _children.size();  }

  virtual void clear();
  virtual LinkedTrieNode *createNode() {
    LinkedTrieNode *node = new LinkedTrieNode();
    return node;
  }

  virtual void traverseDFS(TrieNodeVisitorInterface &visitor);
  virtual void getStringsInSubtrie(const std::string &prefix, 
      std::map<std::string, TrieValueType> &entries);

  void getCharLabels(std::vector<uint8_t> &labels) {
    for (ChildrenMapIterator itr = _children.begin(), end = _children.end();
        itr != end;
        ++itr) {
      labels.push_back(itr->first);
    }
  }

  virtual void set_id(uint32_t id) {  _id = id;  }
  virtual uint32_t get_id() {  return _id;  }

  //LinkedTrieNode *get_parent() {  return _parent;  }
  uint8_t getLabelWithChild(TrieNode *child);

 private:
  uint32_t _id;
  //LinkedTrieNode *_parent;
  boost::ptr_map<uint8_t, TrieNode> _children;
};

#endif

