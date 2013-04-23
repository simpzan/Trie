#include "TrieNode.h"
#include <cstring>
#include <string>
#include <iostream>
#include <typeinfo>

using namespace std;


TrieNode *TrieNode::_searchNodeWithKey(const char *key, int &prefixLength) {

	int size = strlen(key);
	TrieNode *node = this;
	int i=0;
	for (; i<size; ++i) {
		int ch = key[i];

		TrieNode *nextNode =node->getChildNodeWithLabel(ch);
		
		if (!nextNode) {
			break;
		}

		node = nextNode;
	}

	prefixLength = i;
	return node;
}

TrieNode *TrieNode::_addNodesWithKey(const char *key, int startIndex) {

	int size = strlen(key);
	TrieNode *node = this;
	for (int i=startIndex; i< size; ++i) {
		uint8_t ch = key[i];
		TrieNode *newNode = this->createNode();
		node->setChildNodeWithLabel(ch, newNode);

		node = newNode;
	}

	return node;
}


void TrieNode::addEntry(const char *key, uint64_t value) {

	int prefixLength = 0;
	TrieNode *node = this->_searchNodeWithKey(key, prefixLength);

	node = node->_addNodesWithKey(key, prefixLength);

	node->setValue(value);
}

uint64_t TrieNode::getEntry(const char *key) {

	int prefixLen = 0;
	TrieNode *node = this->_searchNodeWithKey(key, prefixLen);

	if (prefixLen != strlen(key) ) {
		return 0;
	}

	return node->getValue();
}

bool TrieNode::prefix_range(const char *pattern, vector<Entry> &entries) {

	int prefixLength = 0;
	TrieNode *node = this->_searchNodeWithKey(pattern, prefixLength);

	int lastIndex = strlen(pattern);
	if (prefixLength != lastIndex) {
		return false;
	}

	uint8_t ch = pattern[lastIndex];
	//node = node->getChildNodeWithLabel(ch);

	string prefix(pattern);
	node->_getStringsInSubTrieRecursively(entries, prefix);

	return true;
}


void TrieNode::_getStringsInSubTrieRecursively(vector<Entry> &entries, string &prefix) {
	uint64_t value = getValue();
	if (value) {
		Entry entry(prefix, value);
		entries.push_back(entry);
	}

	for (int i=0; i<256; ++i) {
		TrieNode *child = getChildNodeWithLabel(i);

		string word = prefix+(char)i;
		if (child) {
			child->_getStringsInSubTrieRecursively(entries, word);
		}
	}

}

uint32_t TrieNode::sizeInByte() {
	uint32_t size = this->sizeOfThisNode();
	for (int i=0; i<256; ++i) {
		TrieNode *child = this->getChildNodeWithLabel(i);
		if (child) {
			size += child->sizeInByte();
		}
	}

	return size;
}


uint32_t TrieNode::sizeInMegaByte() {
	return this->sizeInByte() / (1024*1024);
}

