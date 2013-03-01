#ifndef ARRAYTRIE_H
#define ARRARYTRIE_H

#include "Trie.h"
#include "ArrayTrieNode.h"

class ArrayTrie : public Trie {
public:
	ArrayTrie();
	~ArrayTrie();

public:
	virtual bool prefix_range(std::string& pattern, std::vector<std::string>& results);
	virtual bool prefix_range(const char *pattern, std::vector<std::string>& results);

	virtual void addEntry(std::string& key, uint64_t value);
	virtual void addEntry(const char* key, uint64_t value);
	virtual uint64_t getEntry(std::string &key);
	virtual uint64_t getEntry(const char *key);

	virtual int64_t rank(std::string& key);

	virtual	std::string select(uint64_t index);

private:
	std::vector<ArrayTrieNode *> _nodes;
	ArrayTrieNode *_root;

private:
	// create a new node and record in _nodes,
	ArrayTrieNode *_newNode();
	// search in the trie from root with key. return lowest node and the length of prefix match.
	ArrayTrieNode *_searchNodeWithKey(std::string &key, int &prefixLength);
	// add nodes with key[startIndex, key.size()-1], return the last added node.
	ArrayTrieNode *_addNodesWithKey(std::string &key, int startIndex, ArrayTrieNode *startNode);
};

#endif
