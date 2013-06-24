
#include "PTrieNode.h"
#include <algorithm>

using namespace std;

int PTrieNode::counter = 0;
bool startsWith(const char *key, const char *prefix) {
  int key_len = strlen(key);
  int prefix_len = strlen(prefix);
  if (key_len < prefix_len)  return false;

  return strncmp(key, prefix, prefix_len) == 0;
}

bool matchLabel(const char *key, PTrieNode *tail_node, int &matched_count) {
  matched_count = 0;
  if (tail_node == NULL)  return true;

  PTrieNode *node = tail_node;
  int count = strlen(key);
  for (int i = 0; i < count; ++i) {
    uint8_t ch = key[i];
    PTrieNode *parent = node->get_parent();
    if (!parent) {
      matched_count = i;
      return true;
    }

    if (parent->getChildNodeWithLabel(ch) != node) {
      return false;
    }

    node = parent;
  }
  if (!node->get_parent()) {
    matched_count = count;
    return true;
  }
  return false; // prefix matched. but we require full match.
}

void PTrieNode::findChildViaLink(const char *key, PTrieNode *&nextNode, int &matched_count) {
  nextNode = NULL;
  matched_count = 0;
  uint8_t ch = key[0];
  PTrieNode *child = getChildNodeWithLabel(ch);
  if (!child) {
    return;
  }

  const char *suffix = key + 1;
  int matched_len;
  bool found = matchLabel(suffix, child->_link_node, matched_len);
  if (!found)  return;

  nextNode = child;
  matched_count = matched_len + 1;
}

void PTrieNode::findChildViaLabel(const char *key, PTrieNode *&nextNode, int &matched_count) {
  uint8_t ch = key[0];
  PTrieNode *child = getChildNodeWithLabel(ch);
  if (!child) {
    nextNode = NULL;
    matched_count = 0;
    return;
  }

  const char *suffix = key + 1;
  const char *label = child->get_label().c_str();
  if (startsWith(suffix, label)) {
    nextNode = child;
    matched_count = strlen(label) + 1;
  } else {
    nextNode = NULL;
    matched_count = 0;
  }
}

uint8_t PTrieNode::getCharLabelWithChild(PTrieNode *child) {
  for (ChildrenMapIterator itr = _children.begin(); itr != _children.end(); ++itr) {
    if (itr->second == child)  return itr->first;
  }
  assert(false);
}

void expandLabel(PTrieNode *parent, PTrieNode *child) {
  string label = child->get_label();
  uint8_t ch = parent->getCharLabelWithChild(child);
  label = (char)ch + label;

  int count = label.size();
  for (int i = 0; i < count - 1; ++i) {
    PTrieNode *new_node = child->createNode();
    parent->setChildNodeWithLabel(label[i], new_node);
    parent = new_node;
  }
  parent->setChildNodeWithLabel(label[count - 1], child);
  child->set_label("");
}

uint32_t PTrieNode::collectNodesRecursively(vector<PTrieNode *> &nodes, PTrieNode *parent) {
  int len_min = 5;
  int len = _label.size();
  uint32_t node_added = 0;
  if (len > len_min) {
    nodes.push_back(this);
  } else if (len > 0){
    expandLabel(parent, this);
    node_added = len;
  }

  for (ChildrenMapIterator itr = _children.begin(); itr != _children.end(); ++itr) {
    node_added += itr->second->collectNodesRecursively(nodes, this);
  }
  return node_added;
}

void PTrieNode::traverseDFS(TrieNodeVisitorInterface &visitor) {
  visitor.visitNode(*this);

  for (ChildrenMapIterator itr = _children.begin(); itr != _children.end(); ++itr) {
    itr->second->traverseDFS(visitor);
  }
}

