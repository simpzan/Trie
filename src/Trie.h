#ifndef TRIE_H
#define TRIE_H

#include <utility>
#include <string>
#include <stdint.h>
#include <vector>
#include <assert.h>
#include <cstring>
#include "TrieNode.h"

class Trie {

public:
	Trie(TrieNode *node) : _root(node) {};
	
	virtual ~Trie() {
		delete _root;
	};

public:
	// add an entry with the key/value 
	virtual void addEntry(const char *key, uint64_t value) {
		assert(strlen(key) != 0);
		_root->addEntry(key, value);
	}

	virtual uint64_t getEntry(const char *key) {
		if (strlen(key) == 0) {
			return 0;
		}

		return _root->getEntry(key);
	}

	// prefix search the pattern 
	virtual bool prefix_range(const char *pattern, std::vector<Entry>& results) {
		return _root->prefix_range(pattern, results);
	}
	
	virtual uint32_t sizeInByte() {
		return _root->sizeInByte();
	}

	virtual uint32_t sizeInMegaByte() {
		return _root->sizeInMegaByte();
	}

	// rank of the pattern
	virtual int64_t rank(std::string& key) {};
	// select i-th entry in the trie.
	virtual	std::string select(uint64_t index) {};

	TrieNode *root() {
		return _root;
	}
private:
	TrieNode *_root;
};

#endif
