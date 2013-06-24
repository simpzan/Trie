#include "PTrie.h"
#include <cassert>
#include <queue>
#include "utils.h"
#include "TrieIterator.h"

using namespace std;


void _followKeyViaString(PTrieNode *root, const char *key, 
    PTrieNode *&result, int &matchedCount) {
  PTrieNode *node = root;
  int len_key = strlen(key);
  int total_matched_count = 0;
  while (true) {
    PTrieNode *nextNode;
    int matched_count;
    node->findChildViaLabel(key+total_matched_count, nextNode, matched_count);
    if (!nextNode)  break;

    node = nextNode;
    total_matched_count += matched_count;
    if (total_matched_count >= len_key)  break;
  }
  result = node;
  matchedCount = total_matched_count;
}

void _createSeparatorNode(uint8_t &ch, const char *&suffix, PTrieNode *&node, 
    PTrieNode *child) {
  string label = child->get_label();
  int lcp = computeLCP(label.c_str(), suffix);
  int len_label = label.size();
  assert(lcp < len_label);
  PTrieNode *separator = node->createNode();
  node->setChildNodeWithLabel(ch, separator);
  separator->set_label(label.substr(0, lcp));
  separator->setChildNodeWithLabel(label[lcp], child);
  child->set_label(label.c_str() + lcp + 1);

  ch = suffix[lcp];
  suffix += lcp + 1;
  node = separator;
}

void _createTailNode(uint8_t ch, const char *suffix, PTrieNode *&node) {
  PTrieNode *new_node = node->createNode();
  new_node->set_label(suffix);
  node->setChildNodeWithLabel(ch, new_node);
  node = new_node;
}

void PTrie::addEntry(const char *key, TrieValueType value) {
  PTrieNode *node;
  int matchedCount;
  _followKeyViaString(_root, key, node, matchedCount);

  int key_len = strlen(key);
  assert(matchedCount <= key_len);

  if (matchedCount < key_len) {
    uint8_t ch = key[matchedCount];
    const char *suffix = key + matchedCount + 1;
    PTrieNode *child = node->getChildNodeWithLabel(ch);
    if (child) {
      _createSeparatorNode(ch, suffix, node, child);
      ++_node_count;
    }
    _createTailNode(ch, suffix, node);
    ++_node_count;
  }
  node->setValue(value);
  ++_key_count;
}

inline void PTrie::clear() {
  _root->clear();
  _node_count = 1;
  _key_count = 0;
}


bool PTrie::findEntry(const char *key, TrieValueType &value) {
  value = 0;

  PTrieNode *node;
  int matched_count;
  _followKeyViaString(_root, key, node, matched_count);
  if (matched_count != strlen(key))  return false;

  value = node->getValue();
  return true;
}

void PTrie::collectLabels(LinkedTrie &labels) {
  vector<PTrieNode *> nodes;
  uint32_t node_added = _root->collectNodesRecursively(nodes, NULL);
  _node_count += node_added;

  int count = nodes.size();
  for (int i = 0; i < count; ++i) {
    PTrieNode *node = nodes[i];
    uint32_t link = labels.insertKey(node->get_label().c_str());
    node->set_link(link);
  }
}

void PTrie::collectLabels(std::vector<std::string> &labels) {
  vector<PTrieNode *> nodes;
  uint32_t node_added = _root->collectNodesRecursively(nodes, NULL);
  _node_count += node_added;

  int count = nodes.size();
  for (int i = 0; i < count; ++i) {
    PTrieNode *node = nodes[i];
    labels.push_back(node->get_label());
    uint32_t offset = labels.size();
    node->set_link(offset);
  }
}

bool PTrie::canAddEntry(const char *key, uint32_t value, int block_size) {
  uint32_t node_count = strlen(key) + _node_count;
  uint32_t key_count = 1 + _key_count;

  uint32_t size_dfuds = node_count * 3 / 8;
  uint32_t size_labels = node_count;
  uint32_t size_is_keys = node_count * 1.5 / 8;
  uint32_t size_values = key_count * sizeof(uint32_t) / 3;
  uint32_t size_has_links = node_count * 1.5 / 8;
  uint32_t size_links = key_count * 1 * sizeof(uint32_t) / 3;
  uint32_t size_all = size_dfuds + size_labels + size_is_keys + size_values 
    + size_has_links + size_links;
  bool can = size_all <= block_size;
  if (can)  return true;

  return false;
}

void PTrie::traverseDFS(TrieNodeVisitorInterface &visitor) {
  _root->traverseDFS(visitor);
}

void PTrie::traverseBFS(TrieNodeVisitorInterface &visitor) {
  TrieBfsIterator itr(*this);
  PTrieNode *node;
  while (true) {
    node = (PTrieNode *)itr.next();
    if (node == NULL)  break;

    visitor.visitNode(*node);
  }
}

