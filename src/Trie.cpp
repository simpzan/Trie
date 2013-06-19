#include "Trie.h"

using namespace std;

inline void Trie::clear() {
  _root->clear();
  _node_count = 1;
  _value_count = 0;
}

void Trie::_createNodes(const char *key, TrieNode *node, 
    vector<TrieNode *> &nodes) {
  int count = strlen(key);
  TrieNode *tmp_node = node;
  for (int i = 0; i < count; ++i) {
    TrieNode *new_node = node->createNode();
    tmp_node->setChildNodeWithLabel(key[i], new_node);
    tmp_node = new_node;
    nodes.push_back(new_node);
  }
}

bool Trie::_followKey(const char *key, vector<TrieNode *> &nodes) {
  int count = strlen(key);
  TrieNode *node = _root;
  nodes.push_back(_root);
  for (int i = 0; i < count; ++i) {
    TrieNode *next = node->getChildNodeWithLabel(key[i]);
    if (next == NULL)  return false;

    node = next;
    nodes.push_back(next);
  }
  return true;
}

TrieNode *Trie::addKey(const char *key) {
  assert(strlen(key));

  vector<TrieNode *> nodes;
  bool found = _followKey(key, nodes);
  TrieNode *node = nodes.back();

  int prefixLen = nodes.size() - 1;
  if (!found) {
    const char *suffix = key + prefixLen;
    nodes.clear();
    _createNodes(suffix, node, nodes); 
  }

  int node_count = strlen(key) - prefixLen;
  _node_count += node_count;
  
  return nodes.back();
}

void Trie::addEntry(const char *key, TrieValueType value) {
  TrieNode *newNode = addKey(key);
  newNode->setValue(value);
  ++_value_count;
}

bool Trie::findEntry(const char *key, TrieValueType &value) {
  value = 0;
  if (strlen(key) == 0)  return false;

  vector<TrieNode *> nodes;
  bool found = _followKey(key, nodes);
  if (!found)  return false;

  TrieNode *node = nodes.back();
  value = node->getValue();
  return true;
}


