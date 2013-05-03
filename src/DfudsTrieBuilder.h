#ifndef DFUDS_TRIE_BUILDER_H
#define DFUDS_TRIE_BUILDER_H

#include <vector>
#include <stdint.h>
#include <iostream>

#include "BalancedBitVectorBuilder.h"
#include "Vector.h"
#include "Trie.h"
#include "LinkedTrieNode.h"

class DfudsTrieBuilder : public Trie, public TrieVisitorInterface {
  public:
	DfudsTrieBuilder() : Trie(new LinkedTrieNode) {}
	virtual ~DfudsTrieBuilder() {}

	void buildDfuds();
	virtual void write(std::ostream &os);
	virtual void clear();
	virtual uint64_t size();

	virtual bool visitNode(TrieNode &node);
	
  private:
	void writeVector(std::ostream &os, std::vector<uint8_t> &array);

  private:
	BalancedBitVectorBuilder _dfuds;
	Vector<uint8_t> _labels;
	BitVectorBuilder _is_keys;
};

#endif
