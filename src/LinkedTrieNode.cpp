#include "LinkedTrieNode.h"
#include <cassert>
#include <iostream>

using namespace std;

LinkedTrieNode::~LinkedTrieNode() {
  clear();
}

TrieNode *LinkedTrieNode::getChildNodeWithLabel(uint8_t ch) {
	ChildrenMapIterator itr = _children.find(ch);
	if (itr == _children.end())  return NULL;

	return (TrieNode *)itr->second;
}

uint8_t LinkedTrieNode::getLabelWithChild(TrieNode *child) {
   for (ChildrenMapIterator itr = _children.begin(), end = _children.end();
       itr != end;
       ++itr) {
     if (child == itr->second)  return itr->first;
   }
   assert(false);
}

void LinkedTrieNode::setChildNodeWithLabel(uint8_t ch, TrieNode *node) {
  assert(node);
  //removeChildNodeWithLabel(ch);
	_children.insert(ch, node);
  LinkedTrieNode *child = (LinkedTrieNode *)node;
  //child->_parent = this;
}

void LinkedTrieNode::removeChildNodeWithLabel(uint8_t ch) {
  TrieNode *existing = getChildNodeWithLabel(ch);
  if (existing) {
    delete existing;
    _children.erase(ch);
  }
}

void LinkedTrieNode::clear() {
	_children.clear();
  TrieNode::clear();
}

void LinkedTrieNode::traverseDFS(TrieNodeVisitorInterface &visitor) {
   visitor.visitNode(*this);

   for (ChildrenMapIterator itr = _children.begin(), end = _children.end();
       itr != end;
       ++itr) {
     TrieNode *node = (TrieNode *)itr->second;
     assert(node);
     node->traverseDFS(visitor);
   }
}

void LinkedTrieNode::getStringsInSubtrie(const string &prefix, 
    map<string, TrieValueType> &entries) {
  TrieValueType value = getValue();
  if (value)  entries[prefix] = value;

  for (ChildrenMapIterator itr = _children.begin(), end = _children.end();
      itr != end;
      ++itr) {
    TrieNode *node = (TrieNode *) itr->second;
    assert(node);
    string prefix_new = prefix + (char)itr->first;
    node->getStringsInSubtrie(prefix_new, entries);
  }
}
