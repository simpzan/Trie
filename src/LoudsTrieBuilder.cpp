#include <cassert>
#include <sdsl/util.hpp>
#include "LoudsTrieBuilder.h"
#include "TrieIterator.h"

using namespace sdsl;
using namespace std;


void LoudsTrieBuilder::_preBuild(uint32_t node_count) {
  bit_vector louds(node_count * 2 -1);
  _louds.swap(louds);
  _louds[0] = 1;
  _louds_pos = 2;
  _labels.clear();
}

void LoudsTrieBuilder::visitNode(TrieNodeInterface &node) {
  node.set_id(_louds_pos);

  vector<uint8_t> labels;
  node.getCharLabels(labels);
  int count = labels.size();
  for (int i = 0; i < count; ++i) {
    _louds[_louds_pos] = 1;
    ++_louds_pos;
    _labels.append(labels[i]);
  }
  ++_louds_pos;
}

bool LoudsTrieBuilder::build(TrieInterface &trie) {
  uint32_t node_count = trie.getNodeCount() + 1;
  _preBuild(node_count);

  TrieBfsIterator itr = TrieBfsIterator(trie);
  TrieNodeInterface *node;
  while (true) {
    node = itr.next();
    if (node == NULL)  break;

    visitNode(*node);
  }
  return true;
}

bool LoudsTrieBuilder::load(std::istream &is) {
  _louds.load(is);
  _labels.read(is);
  return true;
}

bool LoudsTrieBuilder::serialize(std::ostream &os) const {
  _louds.serialize(os);
  _labels.write(os);
  return true;
}