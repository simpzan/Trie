#ifndef DFUDS_TRIE_H
#define DFUDS_TRIE_H

#include <vector>
#include "ConstBitVector.h"
#include "ConstBalancedBitVector.h"
#include "ConstVector.h"
#include "Vector.h"
#include "Interface.h"

class DfudsTrie {
 public:
  DfudsTrie() {}
  ~DfudsTrie() {}

  // return the rank of the key in this trie. 
  // return 0, if the key does not exist.
  uint32_t find(const char *key) const;
  uint32_t rightNearFind(const char *key) const;

  // extract the rank-th key in trie.
  // not efficient yet, cause there's no bit vector select index.
  void select(uint32_t rank, std::string &key) const;

  virtual void read(std::istream &is);
  uint32_t mmap(const uint8_t *address);
  void clear();

  void display() const;
  void computeOffsets(const std::string &key, Vector<uint32_t> &offsets) const;
  // return the rank of the key in this trie. 
  // return 0, if the key does not exist.
  virtual uint32_t findLCP(const char *key, uint8_t *lcp) const;
  virtual uint32_t rightNearFindLCP(const char *key, uint8_t *lcp) const;

 protected:
  uint8_t _childRank(uint32_t id, uint8_t ch) const;
  uint32_t _childSelect(uint32_t id, uint8_t rank) const;
  bool _followKey(const char *key, uint32_t &id, int *prefixLen, int *depth = NULL) const;
  uint32_t _keyRank(uint32_t id) const;
  uint8_t _degree(uint32_t id) const;
  uint8_t _childLowerBound(uint32_t id, uint8_t ch) const;
  uint32_t _generalizedSibling(uint32_t id) const;

  uint32_t _parent(uint32_t id) const;
  uint8_t _label(uint32_t parent, uint32_t child) const;

 private:
  ConstBalancedBitVector _dfuds;
  ConstVector<uint8_t> _labels;
  ConstBitVector _is_keys;
};

class DfudsTrieLCP : public DfudsTrie {
  public:
  DfudsTrieLCP() {}
  ~DfudsTrieLCP() {}

  virtual void read(std::istream &is);
  // return the rank of the key in this trie. 
  // return 0, if the key does not exist.
  uint32_t findLCP(const char *key, uint8_t *lcp) const;
  uint32_t rightNearFindLCP(const char *key, uint8_t *lcp) const;

  private:
  ConstVector<uint32_t> _offsets;

};

#endif
