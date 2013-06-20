#include <cassert>
#include <sdsl/util.hpp>
#include "LoudsTrie.h"
#include "Trie.h"

using namespace std;
using namespace sdsl;

void LoudsTrie::_preBuild(uint32_t node_count) {
  bit_vector louds(node_count * 2 -1);
  _louds.swap(louds);
  _louds[0] = 1;
  _louds_pos = 2;
}

void LoudsTrie::visitNode(TrieNodeInterface &node) {
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

//void verify(bit_vector &louds) {
  //uint32_t len = louds.size();
  //assert(louds[len - 1] == 0);
  //assert(louds[len - 2] == 0);
//}

void LoudsTrie::_postBuild() {
  util::init_support(_louds_rank0, &_louds);
  util::init_support(_louds_select0, &_louds);
  util::init_support(_louds_select1, &_louds);
}

bool LoudsTrie::build(TrieInterface &trie) {
  uint32_t node_count = trie.getNodeCount() + 1;
  _preBuild(node_count);

  TrieBfsIterator itr = TrieBfsIterator(trie);
  TrieNodeInterface *node;
  while (true) {
    node = itr.next();
    if (node == NULL)  break;

    visitNode(*node);
  }
  _postBuild();
}

bool LoudsTrie::load(std::istream &is) {
  _louds.load(is);
  _louds_rank0.load(is, &_louds);
  _louds_select0.load(is, &_louds);
  _louds_select1.load(is, &_louds);
  _labels.read(is);
}

bool LoudsTrie::serialize(std::ostream &os) const {
  _louds.serialize(os);
  _louds_rank0.serialize(os);
  _louds_select0.serialize(os);
  _louds_select1.serialize(os);
  _labels.write(os);
}

uint32_t LoudsTrie::startOfNode(uint32_t pos) {
  uint32_t rank = loudsRank0(pos);
  return loudsSelect0(rank) + 1;
}

uint32_t LoudsTrie::parent(uint32_t child, uint32_t &parent, uint8_t &ch) {
  assert(child >= root());
  uint32_t rank0 = loudsRank0(child - 1);
  if (rank0 == 1)  return 0;

  uint32_t pos = loudsSelect1(rank0);
  parent = startOfNode(pos);
  int index = pos - parent + 1;
  ch = getChar(parent, index);
}

uint8_t LoudsTrie::getChar(uint32_t node, int index) {
  uint32_t labels_base = loudsRank1(node) - 1 - 1;
  uint32_t labels_pos = labels_base + index - 1;
  return _labels[labels_pos];
}

uint32_t LoudsTrie::sibling(uint32_t node) {
  uint32_t rank0 = loudsRank0(node - 1);
  if (rank0 == 1)  return 0;

  uint32_t pos = loudsSelect1(rank0);
  if (_louds[pos + 1] == 0)  return 0;

  return loudsSelect0(rank0 + 1) + 1;
}

void LoudsTrie::getCharLowerBound(uint32_t node, uint8_t ch,
    int &index, uint8_t &fetched_ch) {
  uint32_t labels_base = loudsRank1(node) - 1 - 1;
  
  for (int i = 0; _louds[node + i]; ++i) {
    fetched_ch = _labels[labels_base + i];
    if (fetched_ch >= ch) {
      index = i + 1;
      return;
    }
  }
  index = 0;
}

uint32_t LoudsTrie::generalSibling(uint32_t node) {
  uint32_t parent = node;
  uint32_t sibling;
  while (true) {
    sibling = this->sibling(parent);
    if (sibling != 0)  return sibling;
    uint8_t ch;
    this->parent(parent, parent, ch);
    if (parent == root())  return 0;
  }
}

uint32_t LoudsTrie::childSelect(uint32_t node, int index) {
  assert(index > 0);
  int offset = index - 1;
  uint32_t pos = node + offset;
  uint32_t rank1 = loudsRank1(pos);
  uint32_t child = loudsSelect0(rank1) + 1;
  return child;
}

void LoudsTrie::findChildViaChar(uint32_t node, uint8_t ch, 
    uint32_t &child, uint8_t &fetched_ch) {
  int index;
  getCharLowerBound(node, ch, index, fetched_ch);
  if (index == 0) {
    child = 0;
    return;
  }
  child = childSelect(node, index);
}

void LoudsTrie::computePrefix(uint32_t node, std::string &prefix) {
  uint32_t child = node;
  uint32_t parent;
  uint8_t ch;
  while (true) {
    this->parent(child, parent, ch);
    //cout << child << endl;

    prefix += ch;
    if (parent == root())  break;
    child = parent;
  }
}

void LoudsTrie::display() {
  cout << "louds:" << _louds.size() << endl << _louds << endl;
  cout << "labels" << endl;
  _labels.display(cout);
}
