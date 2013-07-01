#ifndef DFUDS_PTRIE_MAP_H
#define DFUDS_PTRIE_MAP_H

#include <sdsl/int_vector.hpp> // for the bit_vector class
#include <sdsl/util.hpp> // for counting the set bits in a bit_vector 
#include <sdsl/rank_support.hpp> // for rank data structures
#include <sdsl/select_support_mcl.hpp>
#include <sdsl/bp_support.hpp>

#include "PTrieNode.h"
#include "Vector.h"
#include "DACWrapper.h"
#include "SuccinctMap.h"
#include "DfudsTrie.h"

using namespace sdsl;

class DfudsMap : public TrieNodeVisitorInterface, public SuccinctMap {
 public:
  DfudsMap() {}
  virtual ~DfudsMap() {}

  virtual bool visitNode(TrieNodeInterface &node);

  virtual bool build(TrieInterface &trie);
  virtual bool load(std::istream &is);
  virtual bool serialize(std::ostream &os);
  virtual void clear() {  assert(false);  }

  // find the value associated with key.
  // return true if found, false otherwise.
  virtual bool findEntry(const char *key, TrieValueType &value);
  // find the first entry whose key is greater than or equal to `pattern`.
  virtual bool findEntryLowerBound(const char *pattern, 
      std::string *key, TrieValueType &value) {  assert(false);  }

  void updateLinks(const vector<uint32_t> &nodeIds); 

 private:
  void _preBuild(int node_count);
  void _postBuild();

  DfudsTrie _trie;

  sdsl::bit_vector _is_keys;
  rank_support_v<> _is_keys_rank_support;
  Vector<uint32_t> _values;
  DACWrapper _values_DAC;

  sdsl::bit_vector _has_links;
  rank_support_v<> _has_links_support;
  Vector<uint32_t> _links;
  DACWrapper _links_DAC;

  uint32_t _is_keys_pos;
  uint32_t _has_links_pos;
};

#endif
