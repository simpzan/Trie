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
	Trie(TrieNode *node) : _root(node), _node_count(1), _value_count(0) {}
	virtual ~Trie() {  delete _root;  }

	// add an entry with the key/value 
	virtual void addEntry(const char *key, uint64_t value);
	virtual uint64_t getEntry(const char *key);

	// prefix search the pattern 
	bool prefixSearch(const char *pattern, std::vector<EntryT>& results);
	virtual uint32_t sizeInByte() {  return _root->sizeInByte();  }
	virtual uint32_t sizeInMegaByte() {	return _root->sizeInMegaByte(); }

	TrieNode *root() {  return _root;  }
	uint64_t node_count() {  return _node_count;  }
	uint64_t value_count() {  return _value_count;  }
	virtual void clear();

  private:
	TrieNode *_root;
	uint64_t _node_count;
	uint64_t _value_count;
};

#endif
