#include <vector>
#include <assert.h>
#include <iostream>
#include "ArrayTrie.h"

using namespace std;

ArrayTrie::ArrayTrie() {
	_root = new ArrayTrieNode;
}

ArrayTrie::~ArrayTrie() {
	delete _root;
	_root = NULL;

}

void ArrayTrie::addEntry(const char*key, uint64_t value) {
	int strLen = strlen(key);
	assert( strLen != 0 );
	_root->addValue(key, value);
}

uint64_t ArrayTrie::getEntry(const char *key) {
	int strLen = strlen(key);
	if (strLen == 0) {
		return 0;
	}

	return _root->getValue(key);
}

bool ArrayTrie::prefix_range(const char *pattern, std::vector<Entry> &results) {
	return _root->prefix_range(pattern, results);
}

uint32_t ArrayTrie::sizeInByte() {
	return _root->sizeInByte();
}

uint32_t ArrayTrie::sizeInMegaByte() {
	return _root->sizeInMegaByte();
}
