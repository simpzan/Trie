#ifndef LINKEDTRIE_H
#define LINKEDTRIE_H

#include "LinkedTrieNode.h"
#include "Trie.h"

class LinkedTrie :public Trie {
  public:
	LinkedTrie() : Trie(new LinkedTrieNode) {}
	virtual ~LinkedTrie() {}
};

#endif

