#ifndef TRIENODE_H
#define TRIENODE_H

#include <string>
#include <vector>
#include <stdint.h>

typedef std::pair<std::string, uint64_t> Entry;

class TrieNode {

	public:
	virtual ~TrieNode() {};

	public:
	// add the key with the value in the trie rooted at this node.
	virtual void addEntry(const char *key, uint64_t value);
	// search and return the value associated with the key in this subtrie.
	virtual uint64_t getEntry(const char *key);

	// search to found all entries whose key is prefixed with the pattern.
	// return true if found any, false otherwise.
	virtual bool prefix_range(const char *pattern, std::vector<Entry> &entries);

	virtual uint32_t sizeInByte();
	virtual uint32_t sizeInMegaByte();

	virtual TrieNode *getChildNodeWithLabel(uint8_t ch) = 0;
	virtual void setChildNodeWithLabel(uint8_t ch, TrieNode* node) = 0;

	virtual uint64_t getValue() = 0;
	virtual void setValue(uint64_t value) = 0;

	virtual TrieNode *createNode() = 0;

	virtual uint32_t sizeOfThisNode() = 0;

	void *findNodeWithKey(const char *key, int &prefixLen) {
		return _searchNodeWithKey(key, prefixLen);
	}

private:
	// recursively found all strings ended in this node. prefix is prepended for each found string.
	void _getStringsInSubTrieRecursively(std::vector<Entry> &entries, std::string &prefix);
	// create and add new nodes for suffix of the key started at the index.
	TrieNode *_addNodesWithKey(const char *key, int startIndex);
	// search to found the lowest node which share prefix with the key. 
	// the length of longest common prefix is returned via 'prefixLength'
	TrieNode *_searchNodeWithKey(const char *key, int &prefixLength);
	
};

#endif
