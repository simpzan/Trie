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
  bool followKey(const char *key, int near_search, 
      uint64_t *id, int *prefixLen) const;
  uint64_t findChild(uint64_t offset, uint8_t ch, int near_search) const;

  ConstBalancedBitVector _dfuds;
  ConstVector<uint8_t> _labels;
  ConstBitVector _is_keys;

  friend class DfudsTrieTest_kk_Test;
};

#endif
