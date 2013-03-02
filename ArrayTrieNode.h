#ifndef ARRAYTRIENODE_H
#define ARRAYTRIENODE_H

#include <stdint.h>
#include <vector>
#include <string>

#define NODESIZE 256 

typedef std::pair<std::string, uint64_t> Entry;

class ArrayTrieNode {
public:

public:
	ArrayTrieNode();
	~ArrayTrieNode();

public:
	// return the child with the char in this node.
	ArrayTrieNode *getChildNodeWithLabel(uint8_t ch);
	// set the child with the char in this node.
	void setChildNodeWithLabel(uint8_t ch, ArrayTrieNode *node);

	// set the value with the char in this node.
	void setValueWithLabel(uint8_t ch, uint64_t value);
	// return the child with char in this node.
	uint64_t getValueWithLabel(uint8_t ch);

	// add the key with the value in the trie rooted at this node.
	void addValue(const char *key, uint64_t value);
	// search and return the value associated with the key in this subtrie.
	uint64_t getValue(const char *key);

	// search to found all entries whose key is prefixed with the pattern.
	// return true if found any, false otherwise.
	bool prefix_range(const char *pattern, std::vector<Entry> &entries);

	uint32_t sizeInByte();
	uint32_t sizeInMegaByte();

private:
	// recursively found all strings ended in this node. prefix is prepended for each found string.
	void _getStringsInSubTrieRecursively(std::vector<Entry> &entries, std::string &prefix);
	// create and add new nodes for suffix of the key started at the index.
	ArrayTrieNode *_addNodesWithKey(const char *key, int startIndex);
	// search to found the lowest node which share prefix with the key. 
	// the length of longest common prefix is returned via 'prefixLength'
	ArrayTrieNode *_searchNodeWithKey(const char *key, int &prefixLength);
	
private:
	uint64_t _values[NODESIZE];
	ArrayTrieNode *_chilren[NODESIZE]; 	// children links
};

#endif
