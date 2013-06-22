#ifndef LOUDS_MAP_H
#define LOUDS_MAP_H

#include <sdsl/bit_vectors.hpp>
#include "SuccinctMap.h"
#include "TrieNodeInterface.h"
#include "Vector.h"
#include "PTrieNode.h"
#include "LoudsTrie.h"

class LoudsMap : public SuccinctMap, public TrieNodeVisitorInterface {
 public:
  LoudsMap() {}
  virtual ~LoudsMap() {}

  virtual bool visitNode(TrieNodeInterface &node);

  virtual bool build(TrieInterface &trie);
  virtual bool load(std::istream &is);
  virtual bool serialize(std::ostream &os);

  // find the value associated with key.
  // return true if found, false otherwise.
  virtual bool findEntry(const char *key, TrieValueType &value);
  // find the first entry whose key is greater than or equal to `pattern`.
  virtual bool findEntryLowerBound(const char *pattern, 
      std::string *key, TrieValueType &value);

  void set_label_trie(LoudsTrie *trie) {  _label_trie = trie;  }
  void updateLinks(const std::vector<uint32_t> &nodeIds);
  void display();

 private:
  void _followKey(const char *key, uint32_t &node, uint32_t &matched_count);
  void findChild(uint32_t node, const char *key, uint32_t &child, uint32_t &matched_count);
  uint32_t findChildLowerBound(uint32_t node, const char *key);
  uint32_t leftMostChild(uint32_t node);

  void _preBuild(uint32_t node_count);
  void _postBuild();
  void computeLabel(uint32_t node, std::string &label);
  uint32_t getLink(uint32_t node);
  TrieValueType getValue(uint32_t node);
  void computeKey(string &key, uint32_t node);

  void findChildViaChar(uint32_t node, uint8_t ch,
      uint32_t &child, uint8_t &fetched_ch) {
    return _trie.findChildViaChar(node, ch, child, fetched_ch);
  }
  uint32_t childSelect(uint32_t node, int index) {
    return _trie.childSelect(node, index);
  }
  void getCharLowerBound(uint32_t node, uint8_t ch,
    int &index, uint8_t &fetched_ch) { 
    return _trie.getCharLowerBound(node, ch, index, fetched_ch);  
  }
  uint32_t root() {  return _trie.root(); }
  uint32_t sibling(uint32_t node) {  return _trie.sibling(node);  }
  uint32_t generalSibling(uint32_t node) {  return _trie.generalSibling(node);  }

  uint32_t loudsRank0(uint32_t pos) {
    return _trie.loudsRank0(pos);
  }
  uint32_t isTailsRank1(uint32_t pos) {
    return _is_tails_rank1(pos + 1);
  }
  uint32_t hasLinksRank1(uint32_t pos) {
    return _has_links_rank1(pos + 1);
  }

  sdsl::bit_vector _is_tails;
  sdsl::bit_vector::rank_1_type _is_tails_rank1;
  Vector<uint32_t> _values;

  sdsl::bit_vector _has_links;
  sdsl::bit_vector::rank_1_type _has_links_rank1;
  Vector<uint32_t> _links;

  // pos used when build bit_vectors.
  uint32_t _is_tails_pos;
  uint32_t _has_links_pos;

  LoudsTrie *_label_trie;
  LoudsTrie _trie;
};

#endif
