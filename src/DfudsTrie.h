#ifndef DFUDS_TRIE_H
#define DFUDS_TRIE_H

#include <vector>
#include "ConstBitVector.h"
#include "ConstBalancedBitVector.h"
#include "ConstVector.h"
#include "Interface.h"

class DfudsTrie {
 public:
  DfudsTrie() {}
  ~DfudsTrie() {}

  // return the rank of the key in this trie. 
  // return 0, if the key does not exist.
  uint64_t find(const char *key) const;
  uint64_t rightNearFind(const char *key) const;

  // extract the rank-th key in trie.
  // not efficient yet, cause there's no bit vector select index.
  void select(uint64_t rank, std::string &key) const;

  void read(std::istream &is);
  uint32_t mmap(const uint8_t *address);
  void clear();

  void display(std::ostream &os) const;

 private:
  uint8_t _degree(uint64_t id) const;
  uint8_t _childRank(uint64_t id, uint8_t ch) const;
  uint8_t _childLowerBound(uint64_t id, uint8_t ch) const;
  uint64_t _childSelect(uint64_t id, uint8_t rank) const;
  uint64_t _keyRank(uint64_t id) const;
  bool _followKey(const char *key, uint64_t *id, int *prefixLen) const;
  uint64_t _generalizedSibling(uint64_t id) const;

  uint64_t _parent(uint64_t id) const;
  uint8_t _label(uint64_t parent, uint64_t child) const;

  ConstBalancedBitVector _dfuds;
  ConstVector<uint8_t> _labels;
  ConstBitVector _is_keys;
};

#endif
