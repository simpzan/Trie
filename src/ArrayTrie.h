#ifndef ARRAYTRIE_H
#define ARRARYTRIE_H

#include "ArrayTrieNode.h"
#include "Trie.h"

class ArrayTrie :public Trie {
public:
	ArrayTrie(): Trie(new ArrayTrieNode) {};
	virtual ~ArrayTrie() {};
};

#endif
