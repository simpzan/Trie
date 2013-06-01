#include "Trie.h"

using namespace std;

void Trie::clear() {
  _root->clear();
  _node_count = 1;
  _value_count = 0;
}

bool Trie::prefixSearch(const char *pattern, 
    std::map<string, TrieValueT> &entries) {
  int prefixLen = 0;
  vector<TrieNode *> nodes;
  bool found = _followKey(pattern, nodes);
  if (!found) return false;

  TrieNode *node = nodes.back();
  node->getStringsInSubtrie(pattern, entries);
  return true;
}

void Trie::createNodes(const char *key, TrieNode *node, 
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

uint32_t Trie::nodeCountAfterInsert(const char *key) {
  vector<TrieNode *> nodes;
  _followKey(key, nodes);
  uint32_t inserting = strlen(key) - (nodes.size() - 1);
  uint32_t existing = node_count();
  return existing + inserting;
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

void Trie::undoAdd() {
  vector<TrieNode *> nodes;
  bool found = _followKey(_last_key.c_str(), nodes);
  assert(found);

  TrieNode *node = NULL;
  int i = nodes.size() - 1;
  for (; i >= 0; ++i) {
    node = nodes[i];
    if (node->childCount() > 1)  break;
  }
  char ch = _last_key[i];
  assert(node->getChildNodeWithLabel(ch) == node);
  node->setChildNodeWithLabel(ch, NULL);

  TrieNode *node_to_delete = nodes[i+1];
  node_to_delete->clear();
}

void Trie::addEntry(const char *key, uint64_t value) {
  assert(strlen(key));

  vector<TrieNode *> nodes;
  bool found = _followKey(key, nodes);
  TrieNode *node = nodes.back();

  int prefixLen = nodes.size() - 1;
  if (!found) {
    const char *suffix = key + prefixLen;
    nodes.clear();
    createNodes(suffix, node, nodes); 
  }
  TrieNode *newNode = nodes.back();
  newNode->setValue(value);

  int node_count = strlen(key) - prefixLen;
  _node_count += node_count;
  ++_value_count;

  _last_key = key;
}

uint64_t Trie::getEntry(const char *key) {
  if (strlen(key) == 0) {
    return 0;
  }

  vector<TrieNode *> nodes;
  bool found = _followKey(key, nodes);
  if (!found)  return 0;

  TrieNode *node = nodes.back();
  return node->getValue();
}


