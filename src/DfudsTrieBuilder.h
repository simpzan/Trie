#ifndef DFUDS_TRIE_BUILDER_H
#define DFUDS_TRIE_BUILDER_H

#include <stdint.h>
#include <vector>
#include <iostream>
#include <sstream>

#include "BalancedBitVectorBuilder.h"
#include "Vector.h"
#include "Trie.h"
#include "LinkedTrieNode.h"
#include "DACWrapper.h"

class DfudsTrieBuilder : public Trie, public TrieVisitorInterface {
 public:
  DfudsTrieBuilder() : Trie(new LinkedTrieNode) {}
  virtual ~DfudsTrieBuilder() {}

  void buildDfuds();
  virtual void write(std::ostream &os);
  virtual void clear();
  virtual uint64_t sizeWithNewNodeCount(uint32_t count);

  virtual bool visitNode(TrieNode &node);

 private:
  BalancedBitVectorBuilder _dfuds;
  Vector<uint8_t> _labels;
  BitVectorBuilder _is_keys;
};

class DfudsTrieLCPBuilder : public DfudsTrieBuilder {
  public:
  DfudsTrieLCPBuilder() {}
  ~DfudsTrieLCPBuilder() {}

  virtual void write(std::ostream &os);

  private:
  void _buildWriteOffsets(std::stringstream &ss);
  
};

#endif
