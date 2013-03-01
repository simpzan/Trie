#include <vector>
#include <assert.h>
#include <iostream>
#include "ArrayTrie.h"

typedef std::vector<ArrayTrieNode *>::const_iterator NodesIterator;

using namespace std;

ArrayTrieNode* ArrayTrie::_newNode() {

	ArrayTrieNode *node = new ArrayTrieNode();
	_nodes.push_back(node);

	cout << __func__ << " nodes: " << _nodes.size() << endl;

	return node;
}

ArrayTrie::ArrayTrie() {

	_root = _newNode();
}

ArrayTrie::~ArrayTrie() {

	for(NodesIterator itr = _nodes.begin(), end = _nodes.end();
			itr != end;
			++itr) {
		delete *itr;
	}
}

void ArrayTrie::addEntry(std::string& key, uint64_t value) {
	cout << __func__ << " " << key << " " << value << endl;
	assert( key.size() != 0 );

	int matchedLen = 0;
	ArrayTrieNode *node = _searchNodeWithKey(key, matchedLen);

	node = _addNodesWithKey(key, matchedLen, node);

	int ch = key[key.size()-1];
	node->setValueWithLabel(ch, value);
}

ArrayTrieNode *ArrayTrie::_addNodesWithKey(std::string &key, int startIndex, ArrayTrieNode *startNode) {
	ArrayTrieNode *node = startNode;
	int ch = key[startIndex];
	int strLen = key.size();
	for (int i=startIndex; i<strLen-1; ++i) {
		ch = key[i];
		ArrayTrieNode *newNode = _newNode();
		node->setChildNodeWithLabel(ch, newNode);
		node = newNode;
	}	
	return node;
}

void ArrayTrie::addEntry(const char *key, uint64_t value) {
	std::string str(key);
	addEntry(str, value);
}

uint64_t ArrayTrie::getEntry(std::string &key) {
	if (key.size() == 0) {
		return 0;
	}

	int matchedLength = 0;
	ArrayTrieNode *node = _searchNodeWithKey(key, matchedLength);

	if (matchedLength != key.size() -1 ) {
		return 0;
	}

	int ch = key[key.size()-1];
	return node->getValueWithLabel(ch);
}

ArrayTrieNode *ArrayTrie::_searchNodeWithKey(std::string &key, int &matchedLength) {

	ArrayTrieNode *node = _root;
	char ch = key[0];
	int i;
	for (i=0; i<key.size()-1; ++i) {
		ch = key[i];
		ArrayTrieNode *nextNode = node->findChildNodeWithLabel(ch);
		if (!nextNode) {
			break;
		}
		node = nextNode;
	}

	if (i == key.size()-1) {	
		matchedLength = i;
	} else {
		matchedLength = i;
	}

	return node;
}

uint64_t ArrayTrie::getEntry(const char *key) {
	std::string str(key);
	return getEntry(str);
}

bool ArrayTrie::prefix_range(std::string& pattern, std::vector<std::string>& results) {



	return true;
}

bool ArrayTrie::prefix_range(const char *pattern, std::vector<std::string> &results) {
	std::string str(pattern);
	return prefix_range(str, results);
}

int64_t ArrayTrie::rank(std::string& key) {

	return 0;
}

std::string ArrayTrie::select(uint64_t index) { 
	return "";
}
