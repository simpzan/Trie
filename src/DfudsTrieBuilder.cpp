#include <map>
#include "DfudsTrieBuilder.h"
#include "DfudsTrie.h"

using namespace std;

void DfudsTrieBuilder::buildDfuds() {
  _dfuds.append(false);
  traversePreorderly(*this);
}

bool DfudsTrieBuilder::visitNode(TrieNode *aNode) {
  LinkedTrieNode *node = (LinkedTrieNode *)aNode;
  vector<uint8_t> labels;
  node->getLabels(labels);
  int count = labels.size();
  for (int i = 0; i < count; ++i) {
    _dfuds.append(false);
    _labels.append(labels[i]);
  }
  _dfuds.append(true);

  bool isTerminal = node->getValue() != 0;
  _is_keys.append(isTerminal);
}

void DfudsTrieBuilder::write(ostream &os) {
  buildDfuds();

  _dfuds.write(os);
  _labels.write(os);
  _is_keys.write(os);
}

void DfudsTrieBuilder::clear() {
  Trie::clear();
  _dfuds.clear();
  _labels.clear();
  _is_keys.clear();
}

uint32_t DfudsTrieBuilder::sizeWithNodeCount(uint32_t count) {
  int count_nodes = count;
  int size_dfuds = BalancedBitVectorBuilder::sizeWithBitcount(count_nodes * 2);
  int size_is_teminal = BitVectorBuilder::sizeWithBitcount(count_nodes);
  int size_labels = Vector<uint8_t>::sizeWithCount(count_nodes - 1);
  return size_dfuds + size_is_teminal + size_labels;
}

void DfudsTrieLCPBuilder::_buildWriteOffsets(stringstream &ss) {
  string key = Trie::get_last_key();
  Vector<uint32_t> offsets(key.size());

  DfudsTrie trie;
  trie.read(ss);
  trie.computeOffsets(key, offsets);
  offsets.write(ss);
}

void DfudsTrieLCPBuilder::write(ostream &os) {
  stringstream ss;
  DfudsTrieBuilder::write(ss);
  _buildWriteOffsets(ss);

  string str = ss.str();
  os.write(str.c_str(), str.size());
}
