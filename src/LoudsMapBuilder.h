#ifndef LOUDS_MAP_BUILDER_H
#define LOUDS_MAP_BUILDER_H

#include <sdsl/bit_vectors.hpp>
#include "TrieInterface.h"
#include "Vector.h"
#include "LoudsTrieBuilder.h"

class LoudsMapBuilder {
 public:
  LoudsMapBuilder() {}
  ~LoudsMapBuilder() {}

  virtual bool visitNode(TrieNodeInterface &node);

  virtual bool build(TrieInterface &trie);
  virtual bool load(std::istream &is);
  virtual bool serialize(std::ostream &os);

  void updateLinks(const std::vector<uint32_t> &nodeIds);
  void updateValues(const std::vector<uint32_t> &offsets);

  sdsl::bit_vector &is_tails() {  return _is_tails;  }
  Vector<uint32_t> &values() {  return _values;  }
  sdsl::bit_vector &has_links() {  return _has_links;  }
  Vector<uint32_t> &links() {  return _links;  }
  LoudsTrieBuilder &trie() {  return _trie;  }

 private:
  void _preBuild(uint32_t node_count);

  sdsl::bit_vector _is_tails;
  Vector<uint32_t> _values;
  uint32_t _is_tails_pos;

  sdsl::bit_vector _has_links;
  Vector<uint32_t> _links;
  uint32_t _has_links_pos;

  LoudsTrieBuilder _trie;
};

#endif
