#ifndef DFUDS_TRIE_H
#define DFUDS_TRIE_H

#include <sdsl/int_vector.hpp> // for the bit_vector class
#include <sdsl/bit_vectors.hpp>
#include <sdsl/util.hpp> // for counting the set bits in a bit_vector 
#include <sdsl/rank_support.hpp> // for rank data structures
#include <sdsl/select_support_mcl.hpp>
#include <sdsl/bp_support.hpp>

#include "PTrieNode.h"
#include "Vector.h"
#include "DACWrapper.h"
#include "SuccinctMap.h"

typedef sdsl::bp_support_sada<256, 32, sdsl::rank_support_v<>, sdsl::select_support_mcl<0, 1> > dfuds_support;

class DfudsTrie : public TrieNodeVisitorInterface {
 public:
  DfudsTrie() {}
  virtual ~DfudsTrie() {}

  virtual bool visitNode(TrieNodeInterface &node);
  bool build(TrieInterface &trie); 
  bool load(std::istream &is);
  bool serialize(std::ostream &os) const; 
  void clear(); 

  void computePrefix(uint32_t node, std::string &prefix);
  void display();

 private:
  void _preBuild(int node_count); 
  void _postBuild(); 

  sdsl::bit_vector _dfuds;
  dfuds_support _dfuds_support;
  Vector<uint8_t> _labels;

  uint32_t _dfuds_pos;

  friend class DfudsMap;
  friend class DfudsPTrieMap;
};

#endif
