#ifndef TRIE_NODE_INTERFACE_H
#define TRIE_NODE_INTERFACE_H

#include <stdint.h>
#include <vector>

typedef uint32_t TrieValueType;

class TrieNodeInterface {
  public:
  TrieNodeInterface() {}
  virtual ~TrieNodeInterface() {}

  virtual TrieValueType getValue() = 0;
  virtual void setValue(TrieValueType value) = 0;

  virtual void getCharLabels(std::vector<uint8_t> &labels) = 0;
  virtual TrieNodeInterface *getChildWithCharLabel(uint8_t ch) = 0;

  virtual void set_id(uint32_t id) = 0;
  virtual uint32_t get_id() = 0;
};

class TrieNodeVisitorInterface {
 public:
  TrieNodeVisitorInterface() {}
  virtual ~TrieNodeVisitorInterface() {}

  virtual bool visitNode(TrieNodeInterface &node) = 0;
};

#endif
