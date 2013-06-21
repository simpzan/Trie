#include "TrieIterator.h"

using namespace std;

TrieBfsIterator::TrieBfsIterator(TrieInterface &trie) : _trie(trie) {
  _nodes.push(trie.root());
}

TrieNodeInterface *TrieBfsIterator::next() {
  if (_nodes.empty())  return NULL;

  TrieNodeInterface *node = _nodes.front();
  _nodes.pop();

  vector<uint8_t> labels;
  node->getCharLabels(labels);

  int count = labels.size();
  for (int i = 0; i < count; ++i) {
    TrieNodeInterface *child = node->getChildWithCharLabel(labels[i]);
    _nodes.push(child);
  }

  return node;
}
