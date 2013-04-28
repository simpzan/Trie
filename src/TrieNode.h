#ifndef TRIENODE_H
#define TRIENODE_H

#include <string>
#include <vector>
#include <stdint.h>

class TrieNode;
typedef std::pair<std::string, uint64_t> EntryT;

class TrieVisitorInterface {
  public:
	TrieVisitorInterface() {}
	virtual ~TrieVisitorInterface() {}

	virtual bool visitNode(TrieNode &node) = 0;
};

class TrieNode {
  public:
	virtual ~TrieNode() {};

  public:
	virtual uint32_t sizeInByte();
	virtual uint32_t sizeInMegaByte();
	virtual uint32_t sizeOfThisNode() = 0;

	virtual TrieNode *getChildNodeWithLabel(uint8_t ch) = 0;
	virtual void setChildNodeWithLabel(uint8_t ch, TrieNode* node) = 0;

	virtual uint64_t getValue() = 0;
	virtual void setValue(uint64_t value) = 0;
	virtual void clear() = 0;

	virtual TrieNode *createNode() = 0;

	void traversePreorderly(TrieVisitorInterface &visitor);
	int countNewNodeWillCreatedWhenInsertKey(const char *key);

	bool findNodeWithKey(const char *key, TrieNode **node, int *prefixLen);
	void getStringsInSubtrie(const char *prefix, std::vector<EntryT> *entries);
	TrieNode *createNodesWithKey(const char *suffix);
};

#endif
