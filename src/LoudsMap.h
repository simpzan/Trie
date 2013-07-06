#ifndef LOUDS_MAP_H
#define LOUDS_MAP_H

#include <sdsl/int_vector.hpp>
#include <sdsl/rrr_vector.hpp>
#include "LoudsMapBuilder.h"
#include "SuccinctMap.h"
#include "TrieNodeInterface.h"
#include "Vector.h"
#include "PTrieNode.h"
#include "DACWrapper.h"

template <class BitVector>
class LoudsTrie;

template <typename BitVector = sdsl::rrr_vector<>,
         typename Container = DACWrapper,
         typename TrieT = LoudsTrie<sdsl::rrr_vector<> > >
class LoudsMap : public SuccinctMap {
 public:
  LoudsMap() {}
  virtual ~LoudsMap() {}

  template <class LoudsMapT>
  void init(LoudsMapT &builder);
  virtual bool load(std::istream &is);
  virtual bool serialize(std::ostream &os);

  // find the value associated with key.
  // return true if found, false otherwise.
  virtual bool findEntry(const char *key, TrieValueType &value);
  // find the first entry whose key is greater than or equal to `pattern`.
  virtual bool findEntryLowerBound(const char *pattern, 
      std::string *key, TrieValueType &value);

  void set_label_trie(TrieT *trie) {  _label_trie = trie;  }
  void display();

  sdsl::bit_vector &is_tails() {  return _is_tails;  }
  Vector<uint32_t> &values() {  return _values;  }
  sdsl::bit_vector &has_links() {  return _has_links;  }
  Vector<uint32_t> &links() {  return _links;  }
  LoudsTrie<BitVector> &trie() {  return _trie;  }
  void updateValues(std::map<uint32_t, uint32_t> &offsets) {
    std::vector<uint32_t> values(offsets.size());
    int count = _values.count();
    for (int i = 0; i < count; ++i) {
      values[i] = offsets[_values[i]];
    }
    Container values_new(values);
    _values.swap(values_new);
  }

 private:
  void _followKey(const char *key, uint32_t &node, uint32_t &matched_count);
  void findChild(uint32_t node, const char *key, uint32_t &child, uint32_t &matched_count);
  uint32_t findChildLowerBound(uint32_t node, const char *key);
  uint32_t leftMostChild(uint32_t node);

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

  uint32_t loudsRank1(uint32_t pos) {
    return _trie.loudsRank1(pos);
  }
 
  uint32_t isTailsRank1(uint32_t pos) {
    return _is_tails_rank1(pos + 1);
  }
  uint32_t hasLinksRank1(uint32_t pos) {
    return _has_links_rank1(pos + 1);
  }

  BitVector _is_tails;
  typename BitVector::rank_1_type _is_tails_rank1;
  Container _values;

  BitVector _has_links;
  typename BitVector::rank_1_type _has_links_rank1;
  Container _links;

  TrieT *_label_trie;
  LoudsTrie<BitVector> _trie;
};

#include "LoudsMap.hxx"

typedef LoudsMap<> LoudsMapCompressed;
typedef LoudsMap<sdsl::bit_vector, Vector<uint32_t>, LoudsTrie<sdsl::bit_vector> >
LoudsMapUncompressed;

#endif
