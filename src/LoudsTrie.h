#ifndef LOUDS_TRIE_H
#define LOUDS_TRIE_H

#include <sdsl/bit_vectors.hpp>
#include <iostream>
#include "Vector.h"
#include "TrieInterface.h"

class LoudsTrie {
 public:
  LoudsTrie() {}
  ~LoudsTrie() {}

  bool build(TrieInterface &trie);
  bool load(std::istream &is);
  bool serialize(std::ostream &os) const;
  void clear();

  void computePrefix(uint32_t node, std::string &prefix);
  void display();

 protected:
  void _preBuild(uint32_t node_count);
  void visitNode(TrieNodeInterface &node);
  void _postBuild();

  void parent(uint32_t child, uint32_t &parent, uint8_t &ch);
  uint8_t getChar(uint32_t node, int index);
  uint32_t startOfNode(uint32_t pos);
  uint32_t root() {  return 2;  }
  uint32_t sibling(uint32_t node);
  void getCharLowerBound(uint32_t node, uint8_t ch,
    int &index, uint8_t &fetched_ch);
  uint32_t generalSibling(uint32_t node);
  uint32_t childSelect(uint32_t node, int index);
  void findChildViaChar(uint32_t node, uint8_t ch,
      uint32_t &child, uint8_t &fetched_ch);

  uint32_t loudsRank1(uint32_t pos) {
    uint32_t rank0 = loudsRank0(pos);
    return pos + 1 - rank0;
  }
  uint32_t loudsRank0(uint32_t pos) {
    return _louds_rank0(pos + 1);
  }
  uint32_t loudsSelect0(uint32_t rank) {
    return _louds_select0(rank);
  }
  uint32_t loudsSelect1(uint32_t rank) {
    return _louds_select1(rank);
  }

 private:
  sdsl::bit_vector _louds;
  sdsl::bit_vector::rank_0_type _louds_rank0;
  sdsl::bit_vector::select_1_type _louds_select1;
  sdsl::bit_vector::select_0_type _louds_select0;
  Vector<uint8_t> _labels;

  uint32_t _louds_pos;
  friend class LoudsMap;
};

inline void LoudsTrie::clear() {
  _louds.resize(0);
  _labels.clear();
  _louds_pos = 0;
}

void showOffset(std::ostream &os);
void showOffset(std::istream &is);
template <class T>
double ratio(const T &rank)  {
  uint32_t bitCount = rank.size();
  uint32_t ones = rank.rank(bitCount - 1);
  double r = (double)ones/ bitCount;

  return r;
}
#endif
