#ifndef ARRAYTRIE_H
#define ARRARYTRIE_H

#include "ArrayTrieNode.h"

class ArrayTrie  {
public:
	ArrayTrie();
	~ArrayTrie();

public:
	virtual bool prefix_range(const char *pattern, std::vector<Entry>& results);

	virtual void addEntry(const char* key, uint64_t value);
	virtual uint64_t getEntry(const char *key);

	uint32_t sizeInByte();
	uint32_t sizeInMegaByte();
private:
	ArrayTrieNode *_root;

private:
};

#endif
