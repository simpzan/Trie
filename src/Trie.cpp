#include "Trie.h"

using namespace std;

namespace {

}

void Trie::clear() {
	_root->clear();
	_node_count = 1;
	_value_count = 0;
}

bool Trie::prefixSearch(const char *pattern, 
		std::vector<EntryT> &entries) {
	int prefixLen = 0;
	TrieNode *node;
    bool found = _root->findNodeWithKey(pattern, &node, &prefixLen);
	if (!found) return false;

	node->getStringsInSubtrie(pattern, &entries);
	return true;
}

void Trie::addEntry(const char *key, uint64_t value) {
	assert(strlen(key));

	int prefixLen = 0;
	TrieNode *node;
   	_root->findNodeWithKey(key, &node, &prefixLen);

	const char *suffix = key + prefixLen;
	TrieNode *newNode = node->createNodesWithKey(suffix);
	
	newNode->setValue(value);

	int node_count = strlen(key) - prefixLen;
	_node_count += node_count;
	++_value_count;

  _largest_key = key;
}

uint64_t Trie::getEntry(const char *key) {
	if (strlen(key) == 0) {
		return 0;
	}

	int prefixLen = 0;
	TrieNode *node;
   	_root->findNodeWithKey(key, &node, &prefixLen);
	if (prefixLen != strlen(key)) return 0;

	return node->getValue();
}


