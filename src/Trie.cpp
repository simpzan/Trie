#include "Trie.h"
#include "utils.h"

using namespace std;

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

void Trie::computePrefix(TrieNode *node, std::string &label) {
  LinkedTrieNode *aNode = (LinkedTrieNode *)node;
  LinkedTrieNode *parent;
  while (1) {
    parent = aNode->get_parent();
    if (parent == NULL)  break;

    uint8_t ch = parent->getLabelWithChild(aNode);
    label = (char)ch + label;
    aNode = parent;
  }
}

