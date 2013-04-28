#include "LinkedTrieNode.h"
#include <iostream>

using namespace std;

typedef map<uint8_t, TrieNode*>::const_iterator ChildrenMapIterator;
typedef map<uint8_t, uint64_t>::const_iterator ValuesMapIterator;

namespace {

void destory(map<uint8_t, TrieNode *> &children) {
	for (ChildrenMapIterator itr = children.begin(), end = children.end();
			itr!=end;
			++itr) {
		TrieNode *child = itr->second;
		delete child;
	}
}

} // namespace


LinkedTrieNode::~LinkedTrieNode() {
	::destory(_children);
}

TrieNode *LinkedTrieNode::getChildNodeWithLabel(uint8_t ch) {
	ChildrenMapIterator itr = _children.find(ch);
	if (itr == _children.end()) {
		return NULL;
	}

	return itr->second;
}

void LinkedTrieNode::setChildNodeWithLabel(uint8_t ch, TrieNode *node) {
	_children[ch] = node;
}

void LinkedTrieNode::clear() {
	::destory(_children);
	_children.clear();
	_value = 0;
}
