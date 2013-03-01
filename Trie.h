#ifndef TRIE_H
#define TRIE_H

#include <utility>
#include <string>
#include <stdint.h>
#include <vector>

class Trie {

public:
	virtual ~Trie() {};

public:
	// add an entry with the key/value 
	virtual void addEntry(std::string& key, uint64_t value) =0;

	// rank of the pattern
	virtual int64_t rank(std::string& key)=0;

	// prefix search the pattern 
	virtual bool prefix_range(std::string& pattern, std::vector<std::string>& results)=0;

	// select i-th entry in the trie.
	virtual	std::string select(uint64_t index)=0;

};

#endif
