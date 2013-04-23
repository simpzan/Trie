#include "LinkedTrieNode.h"
#include <iostream>

using namespace std;

typedef map<uint8_t, TrieNode*>::const_iterator ChildrenMapIterator;
typedef map<uint8_t, uint64_t>::const_iterator ValuesMapIterator;

LinkedTrieNode::~LinkedTrieNode() {
	for (ChildrenMapIterator itr = _children.begin(), end = _children.end();
		   	itr!=end;
		   	++itr) {
		TrieNode *child = itr->second;
		delete child;
	}
}

TrieNode *LinkedTrieNode::getChildNodeWithLabel(uint8_t ch) {
	ChildrenMapIterator itr = _children.find(ch);
	if (itr == _children.end()) {
		return NULL;
	}

	return itr->second;
}


