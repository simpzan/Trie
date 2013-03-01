#include "ArrayTrieNode.h"
#include <cstring>

ArrayTrieNode::ArrayTrieNode() {
	memset(_values, 0, sizeof(_values));
	memset(_chilren, 0, sizeof(_chilren));	
}

ArrayTrieNode::~ArrayTrieNode() {

}

ArrayTrieNode *ArrayTrieNode::findChildNodeWithLabel(char ch) {
	return _chilren[ch];
}

void ArrayTrieNode::setChildNodeWithLabel(char ch, ArrayTrieNode *node) {
	_chilren[ch] = node;
}

void ArrayTrieNode::setValueWithLabel(char ch, uint64_t value) {
	_values[ch] = value;
}

uint64_t ArrayTrieNode::getValueWithLabel(char ch) {
	return _values[ch];
}
