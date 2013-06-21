#include "DfudsTrie.h"

bool DfudsTrie::visitNode(TrieNodeInterface &node) {
  vector<uint8_t> labels;
  node.getCharLabels(labels);
  int count = labels.size();
  for (int i = 0; i < count; ++i) {
    _dfuds[_dfuds_pos] = 1;
    ++_dfuds_pos;
    _labels.append(labels[i]);
  }
  ++_dfuds_pos;
}

bool DfudsTrie::build(TrieInterface &trie) {
  uint32_t node_count = trie.getNodeCount();
  _preBuild(node_count);
  trie.traverseDFS(*this);
  _postBuild();
}

bool DfudsTrie::load(std::istream &is) {
  _dfuds.load(is);
  _dfuds_support.load(is, &_dfuds);
  _labels.read(is);
}

bool DfudsTrie::serialize(std::ostream &os) const {
  _dfuds.serialize(os);
  _dfuds_support.serialize(os);
  _labels.write(os);
}

void DfudsTrie::clear() {
  _labels.clear();
}

void DfudsTrie::computePrefix(uint32_t node, std::string &prefix) {
  assert(false);
}

void DfudsTrie::display() {

}

void DfudsTrie::_preBuild(int node_count) {
  sdsl::bit_vector bv(2 * node_count);
  _dfuds.swap(bv);
  _dfuds_pos = 1;
}

void DfudsTrie::_postBuild() {
  sdsl::util::init_support(_dfuds_support, &_dfuds);
}
