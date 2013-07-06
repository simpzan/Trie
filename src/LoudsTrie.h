#ifndef LOUDS_TRIE_H
#define LOUDS_TRIE_H

#include <sdsl/bit_vectors.hpp>
#include <iostream>
#include "Vector.h"
#include "TrieInterface.h"
#include "LoudsTrieBuilder.h"
#include "LoudsMap.h"

template <class BitVector = sdsl::rrr_vector<> >
class LoudsTrie {
 public:
  LoudsTrie() {}
  virtual ~LoudsTrie() {}

  template <class LoudsTrieT>
  void init(LoudsTrieT &builder);

  bool load(std::istream &is);
  bool serialize(std::ostream &os) const;

  void computePrefix(uint32_t node, std::string &prefix);
  void display();

  BitVector &louds() {  return _louds;  }
  Vector<uint8_t> &labels() {  return _labels;  }

 private:
  void _computePrefix(uint32_t node, std::string &prefix);
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

  uint32_t degree(uint32_t node) {
    uint32_t id = loudsRank1(node) + 1;
    uint32_t next_node = loudsSelect1(id);
    uint32_t degree = next_node - node - 1;
    return degree;
  }
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
  BitVector _louds;
  typename BitVector::rank_0_type _louds_rank0;
  typename BitVector::select_1_type _louds_select1;
  typename BitVector::select_0_type _louds_select0;
  Vector<uint8_t> _labels;

  template <typename BV, typename C, typename T> friend class LoudsMap;
};

#include "LoudsTrie.hxx"

typedef LoudsTrie<> LoudsTrieCompressed;
typedef LoudsTrie<sdsl::bit_vector> LoudsTrieUncompressed;


#endif
