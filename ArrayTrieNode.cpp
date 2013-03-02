#include "ArrayTrieNode.h"
#include <cstring>
#include <string>
#include <iostream>

using namespace std;

ArrayTrieNode::ArrayTrieNode() {

	memset(_values, 0, sizeof(_values));
	memset(_chilren, 0, sizeof(_chilren));	
}

ArrayTrieNode::~ArrayTrieNode() {
	
	for (int i=0; i<NODESIZE; ++i) {
		delete _chilren[i];
		_chilren[i] = NULL;
	}
}

ArrayTrieNode *ArrayTrieNode::getChildNodeWithLabel(uint8_t ch) {
	return _chilren[ch];
}

void ArrayTrieNode::setChildNodeWithLabel(uint8_t ch, ArrayTrieNode *node) {
	_chilren[ch] = node;
}

void ArrayTrieNode::setValueWithLabel(uint8_t ch, uint64_t value) {
	_values[ch] = value;
}

uint64_t ArrayTrieNode::getValueWithLabel(uint8_t ch) {
	return _values[ch];
}

ArrayTrieNode *ArrayTrieNode::_searchNodeWithKey(const char *key, int &prefixLength) {

	int size = strlen(key);
	ArrayTrieNode *node = this;
	int i=0;
	for (; i<size-1; ++i) {
		int ch = key[i];
		ArrayTrieNode *nextNode = node->getChildNodeWithLabel(ch);
		if (!nextNode) {
			break;
		}

		node = nextNode;
	}

	prefixLength = i;
	return node;
}

ArrayTrieNode *ArrayTrieNode::_addNodesWithKey(const char *key, int startIndex) {

	int size = strlen(key);
	ArrayTrieNode *node = this;
	for (int i=startIndex; i< size-1; ++i) {
		int ch = key[i];
		ArrayTrieNode *newNode = new ArrayTrieNode;
		node->setChildNodeWithLabel(ch, newNode);

		node = newNode;
	}

	return node;
}

void ArrayTrieNode::_getStringsInSubTrieRecursively(vector<Entry> &entries, string &prefix) {
	
	for (int i=0; i<NODESIZE; ++i) {
		uint64_t value = getValueWithLabel(i);
		ArrayTrieNode *child = getChildNodeWithLabel(i);
		if (!child && !value) {
			continue;
		}
		
		string word = prefix+(char)i;
		if (value) {
			Entry entry(word, value);
			entries.push_back(entry);
		}

		if (child) {
			child->_getStringsInSubTrieRecursively(entries, word);
		}
	}

}

void ArrayTrieNode::addValue(const char *key, uint64_t value) {

	int prefixLength = 0;
	ArrayTrieNode *node = this->_searchNodeWithKey(key, prefixLength);

	node = node->_addNodesWithKey(key, prefixLength);

	char ch = key[strlen(key)-1];
	node->setValueWithLabel(ch, value);
}

uint64_t ArrayTrieNode::getValue(const char *key) {

	int prefixLen = 0;
	ArrayTrieNode *node = this->_searchNodeWithKey(key, prefixLen);

	if (prefixLen != strlen(key) -1) {
		return 0;
	}

	char ch = key[strlen(key)-1];
	return node->getValueWithLabel(ch);
}

bool ArrayTrieNode::prefix_range(const char *pattern, vector<Entry> &entries) {

 	int prefixLength = 0;
	ArrayTrieNode *node = this->_searchNodeWithKey(pattern, prefixLength);

	int lastIndex = strlen(pattern) - 1;
	if (prefixLength != lastIndex) {
		return false;
	}
	
	uint8_t ch = pattern[lastIndex];
	node = node->getChildNodeWithLabel(ch);

	string prefix(pattern);
	node->_getStringsInSubTrieRecursively(entries, prefix);

	return true;
}

uint32_t ArrayTrieNode::sizeInByte() {
	
	uint32_t size = sizeof(_values) + sizeof(_chilren);
	for (int i=0; i<NODESIZE; ++i) {
		ArrayTrieNode *child = this->getChildNodeWithLabel(i);
		if (child) {
			size += child->sizeInByte();
		}
	}

	return size;
}

uint32_t ArrayTrieNode::sizeInMegaByte() {
	return this->sizeInByte() / (1024*1024);
}

