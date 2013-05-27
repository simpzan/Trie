#ifndef TRIE_NODE_H
#define TRIE_NODE_H

#include <stdint.h>
#include <string>
#include <vector>

class TrieNode;
typedef std::pair<std::string, uint64_t> EntryT;
typedef uint64_t ValueType;

class TrieVisitorInterface {
 public:
  TrieVisitorInterface() {}
  virtual ~TrieVisitorInterface() {}

  virtual bool visitNode(TrieNode &node) = 0;
};

class TrieNode {
 public:
  TrieNode() {}
  virtual ~TrieNode() {}

  virtual uint32_t sizeInByte();
  virtual uint32_t sizeInMegaByte();
  virtual uint32_t sizeOfThisNode() = 0;

  virtual TrieNode *getChildNodeWithLabel(uint8_t ch) = 0;
  virtual void setChildNodeWithLabel(uint8_t ch, TrieNode* node) = 0;

  virtual ValueType getValue() = 0;
  virtual void setValue(ValueType value) = 0;
  virtual void clear() = 0;

  virtual TrieNode *createNode() = 0;

  void traversePreorderly(TrieVisitorInterface &visitor);
  int countNewNodeWillCreatedWhenInsertKey(const char *key);

  bool findNodeWithKey(const char *key, TrieNode **node, int *prefixLen);
  void getStringsInSubtrie(const char *prefix, std::vector<EntryT> *entries);
  TrieNode *createNodesWithKey(const char *suffix);
};

#endif
