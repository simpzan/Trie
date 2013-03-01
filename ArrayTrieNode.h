#ifndef ARRAYTRIENODE_H
#define ARRAYTRIENODE_H

#include <stdint.h>

#define NODESIZE 256


class ArrayTrieNode {

public:
	ArrayTrieNode();
	~ArrayTrieNode();

public:
	ArrayTrieNode *findChildNodeWithLabel(char ch);
	void setChildNodeWithLabel(char ch, ArrayTrieNode *node);
	void setValueWithLabel(char ch, uint64_t value);
	uint64_t getValueWithLabel(char ch);

	void getStringsInSubTrie(std::vector<std::string> &strings);
	ArrayTrieNode *addNodesWithKey(std::string &key, int startIndex);
	ArrayTrieNode *searchNodeWithKey(std::string &key, int &prefixLength);

private:
	
private:
	uint64_t _values[NODESIZE];
	ArrayTrieNode *_chilren[NODESIZE];
};

#endif
