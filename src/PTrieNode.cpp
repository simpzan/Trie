
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

void PTrieNode::collectLabelNodes(vector<PTrieNode*> &nodes) {
  if (_label.size() != 0) {
    nodes.push_back(this);
  }

  for (ChildrenMapIterator itr = _children.begin(); itr != _children.end(); ++itr) {
    itr->second->collectLabelNodes(nodes);
  }
}

void PTrieNode::traverseDFS(TrieNodeVisitorInterface &visitor) {
  visitor.visitNode(*this);

  for (ChildrenMapIterator itr = _children.begin(); itr != _children.end(); ++itr) {
    itr->second->traverseDFS(visitor);
  }
}

