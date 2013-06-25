#ifndef LOUDS_TRIE_HXX
#define LOUDS_TRIE_HXX

#include <cassert>
#include <sdsl/util.hpp>
#include "LoudsTrie.h"
#include "TrieIterator.h"
#include "utils.h"

using namespace std;
using namespace sdsl;

template <typename BitVector>
void LoudsTrie<BitVector>::init(LoudsTrieBuilder &builder) {
  BitVector louds(builder.louds());
  _louds.swap(louds);

  Vector<uint8_t> labels(builder.labels());
  _labels.swap(labels);

  _postBuild();
}

template <typename BitVector>
void LoudsTrie<BitVector>::_postBuild() {
  util::init_support(_louds_rank0, &_louds);
  util::init_support(_louds_select0, &_louds);
  util::init_support(_louds_select1, &_louds);
}

template <typename BitVector>
bool LoudsTrie<BitVector>::load(std::istream &is) {
  _louds.load(is);
  _louds_rank0.load(is, &_louds);
  _louds_select0.load(is, &_louds);
  _louds_select1.load(is, &_louds);
  _labels.read(is);
  return true;
}

template <typename BitVector>
bool LoudsTrie<BitVector>::serialize(std::ostream &os) const {
  _louds.serialize(os);
  _louds_rank0.serialize(os);
  _louds_select0.serialize(os);
  _louds_select1.serialize(os);
  _labels.write(os);
  return true;
}

template <typename BitVector>
uint32_t LoudsTrie<BitVector>::startOfNode(uint32_t pos) {
  uint32_t rank = loudsRank1(pos);
  return loudsSelect1(rank);
}

template <typename BitVector>
void LoudsTrie<BitVector>::parent(uint32_t child, uint32_t &parent, uint8_t &ch) {
  assert(child >= root());
  if (child == root()) {
    parent = 0;
    return;
  }

  uint32_t rank = loudsRank1(child) - 1;
  uint32_t pos = loudsSelect0(rank);
  parent = startOfNode(pos);
  int index = pos - parent;
  ch = getChar(parent, index);
}

template <typename BitVector>
uint8_t LoudsTrie<BitVector>::getChar(uint32_t node, int index) {
  uint32_t labels_base = loudsRank0(node) - 1;
  uint32_t labels_pos = labels_base + index - 1;
  return _labels[labels_pos];
}

template <typename BitVector>
uint32_t LoudsTrie<BitVector>::sibling(uint32_t node) {
  if (node == root())  return 0;

  uint32_t rank = loudsRank1(node);

  uint32_t pos = loudsSelect0(rank - 1);
  if (_louds[pos + 1] == 1)  return 0;

  return loudsSelect1(rank + 1);
}

template <typename BitVector>
void LoudsTrie<BitVector>::getCharLowerBound(uint32_t node, uint8_t ch,
    int &index, uint8_t &fetched_ch) {
  uint32_t labels_base = loudsRank0(node) - 1;
  
  for (int i = 0; !_louds[node + i + 1]; ++i) {
    fetched_ch = _labels[labels_base + i];
    if (fetched_ch >= ch) {
      index = i + 1;
      return;
    }
  }
  index = 0;
}

template <typename BitVector>
uint32_t LoudsTrie<BitVector>::generalSibling(uint32_t node) {
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

template <typename BitVector>
uint32_t LoudsTrie<BitVector>::childSelect(uint32_t node, int index) {
  assert(index > 0);
  uint32_t pos = node + index;
  uint32_t rank = loudsRank0(pos) + 1;
  uint32_t child = loudsSelect1(rank);
  return child;
}

template <typename BitVector>
void LoudsTrie<BitVector>::findChildViaChar(uint32_t node, uint8_t ch, 
    uint32_t &child, uint8_t &fetched_ch) {
  int index;
  getCharLowerBound(node, ch, index, fetched_ch);
  if (index == 0) {
    child = 0;
    return;
  }
  child = childSelect(node, index);
}

template <typename BitVector>
void LoudsTrie<BitVector>::computePrefix(uint32_t node, std::string &prefix) {
  uint32_t pos = loudsSelect1(node + 1);
  _computePrefix(pos, prefix);
}

template <typename BitVector>
void LoudsTrie<BitVector>::_computePrefix(uint32_t node, std::string &prefix) {
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

template <typename BitVector>
void LoudsTrie<BitVector>::display() {
  cout << "louds:" << _louds.size() << "  " << ratio(_louds_rank0) << endl;
  cout << "labels" << endl;
  _labels.display(cout);
}

#endif
