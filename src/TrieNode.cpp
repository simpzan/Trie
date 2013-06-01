#include "TrieNode.h"
#include <cstring>
#include <string>
#include <iostream>
#include <typeinfo>

using namespace std;

void TrieNode::getStringsInSubtrie(const string &prefix, 
    map<string, TrieValueT> &entries) {
	uint64_t value = getValue();
	if (value) {
    entries[prefix] = value;
	}

	for (int i=0; i<256; ++i) {
		TrieNode *child = getChildNodeWithLabel(i);
		if (child) {
		 	string prefix_new = prefix + (char)i;
			child->getStringsInSubtrie(prefix_new, entries);
		}
	}
}

void TrieNode::traversePreorderly(TrieVisitorInterface &visitor) {
	visitor.visitNode(this);

	for (int i = 0; i < 256; ++i) {
		TrieNode *child = this->getChildNodeWithLabel(i);
		if (child)  child->traversePreorderly(visitor);
	}
}

