#include "LinkedTrieNode.h"
#include <iostream>

using namespace std;
typedef map<uint8_t, TrieNode*>::const_iterator ChildrenMapIterator;

LinkedTrieNode::~LinkedTrieNode() {
  clear();
}

TrieNode *LinkedTrieNode::getChildNodeWithLabel(uint8_t ch) {
	ChildrenMapIterator itr = _children.find(ch);
	if (itr == _children.end())  return NULL;

	return itr->second;
}

void LinkedTrieNode::setChildNodeWithLabel(uint8_t ch, TrieNode *node) {
  TrieNode *existing = getChildNodeWithLabel(ch);
	_children[ch] = node;
}

void LinkedTrieNode::clear() {
	for (ChildrenMapIterator itr = _children.begin(), end = _children.end();
			itr != end;
			++itr) {
		TrieNode *child = itr->second;
    child->clear();
		delete child;
	}
	_children.clear();
  TrieNode::clear();
}
