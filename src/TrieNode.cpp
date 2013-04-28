#include "TrieNode.h"
#include <cstring>
#include <string>
#include <iostream>
#include <typeinfo>

using namespace std;

bool TrieNode::findNodeWithKey(const char *key, TrieNode **node, int *prefixLen) {
	int size = strlen(key);
	TrieNode *aNode = this;
	int i=0;
	for (; i<size; ++i) {
		int ch = key[i];
		TrieNode *nextNode =aNode->getChildNodeWithLabel(ch);
		if (!nextNode) {
			break;
		}
		aNode = nextNode;
	}

	if (prefixLen) *prefixLen= i;
	if (node)  *node = aNode;
	return i == size;
}

TrieNode *TrieNode::createNodesWithKey(const char *key) {
	int size = strlen(key);
	TrieNode *node = this;
	for (int i=0; i< size; ++i) {
		uint8_t ch = key[i];
		TrieNode *newNode = this->createNode();
		node->setChildNodeWithLabel(ch, newNode);

		node = newNode;
	}
	return node;
}

int TrieNode::countNewNodeWillCreatedWhenInsertKey(const char *key) {
	int prefixLen = 0;
	this->findNodeWithKey(key, NULL, &prefixLen);
	return strlen(key) - prefixLen;
}

void TrieNode::getStringsInSubtrie(const char *prefix, vector<EntryT> *entries) {
	uint64_t value = getValue();
	if (value) {
		EntryT entry(prefix, value);
		entries->push_back(entry);
	}

	for (int i=0; i<256; ++i) {
		TrieNode *child = getChildNodeWithLabel(i);
		if (child) {
		 	string word = prefix + (char)i;
			child->getStringsInSubtrie(word.c_str(), entries);
		}
	}
}

void TrieNode::traversePreorderly(TrieVisitorInterface &visitor) {
	visitor.visitNode(*this);

	for (int i = 0; i < 256; ++i) {
		TrieNode *child = this->getChildNodeWithLabel(i);
		if (child)  child->traversePreorderly(visitor);
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
