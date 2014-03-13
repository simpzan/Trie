#ifndef LOUDS_TRIE_BUILDER_H
#define LOUDS_TRIE_BUILDER_H

#include <sdsl/int_vector.hpp>
#include "Vector.h"
#include "TrieInterface.h"

class ILoudsTrieBuilder {
public:
  virtual ~ILoudsTrieBuilder() {}

  virtual sdsl::bit_vector &louds() = 0;
  virtual Vector<uint8_t> &labels() = 0;
};

class LoudsTrieBuilder : public ILoudsTrieBuilder {
 public:
  LoudsTrieBuilder() {}
  virtual ~LoudsTrieBuilder() {}

  bool build(TrieInterface &trie);
  bool load(std::istream &is);
  bool serialize(std::ostream &os) const;
  void display() const;

  virtual sdsl::bit_vector &louds() {  return _louds;  }
  virtual Vector<uint8_t> &labels() {  return _labels;  }

 private:
  void _preBuild(uint32_t node_count);
  void visitNode(TrieNodeInterface &node);

  sdsl::bit_vector _louds;
  Vector<uint8_t> _labels;
  uint32_t _louds_pos;
  uint32_t _node_counter;

  friend class LoudsMapBuilder;
};

#endif
