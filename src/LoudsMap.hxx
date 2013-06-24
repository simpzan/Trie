#ifndef LOUDS_MAP_HXX
#define LOUDS_MAP_HXX

#include <sdsl/util.hpp>
#include "LoudsMap.h"
#include "PTrie.h"
#include "utils.h"
#include "TrieIterator.h"

using namespace std;
using namespace sdsl;

template <typename BitVector, typename Container, typename TrieT>
void LoudsMap<BitVector, Container, TrieT>::init(LoudsMapBuilder &builder) {
  _trie.init(builder.trie());
  BitVector is_tails(builder.is_tails());
  _is_tails.swap(is_tails);
  Container values(builder.values().vector());
  _values.swap(values);
  BitVector has_links(builder.has_links());
  _has_links.swap(has_links);
  Container links(builder.links().vector());
  _links.swap(links);
  _postBuild();
}

template <typename BitVector, typename Container, typename TrieT>
void LoudsMap<BitVector, Container, TrieT>::_postBuild() {
  _trie._postBuild();
  util::init_support(_is_tails_rank1, &_is_tails);
  util::init_support(_has_links_rank1, &_has_links);
}

template <typename BitVector, typename Container, typename TrieT>
void LoudsMap<BitVector, Container, TrieT>::display() {
  _trie.display();
  cout << "is tails: " << _is_tails.size() << "  " << ratio(_is_tails_rank1) << endl
    << _is_tails << endl;
  _values.display(cout);
  cout << "has links: " << _has_links.size() << "  " << ratio(_has_links_rank1) << endl 
    << _has_links << endl;
  _links.display(cout);
}

template <typename BitVector, typename Container, typename TrieT>
bool LoudsMap<BitVector, Container, TrieT>::load(std::istream &is) {
  _trie.load(is);

  _is_tails.load(is);
  _is_tails_rank1.load(is, &_is_tails);
  _values.read(is);

  _has_links.load(is);
  _has_links_rank1.load(is, &_has_links);
  _links.read(is);
}

template <typename BitVector, typename Container, typename TrieT>
bool LoudsMap<BitVector, Container, TrieT>::serialize(std::ostream &os) {
  _trie.serialize(os);

  showOffset(os);
  _is_tails.serialize(os);
  _is_tails_rank1.serialize(os);
  _values.write(os);

  _has_links.serialize(os);
  _has_links_rank1.serialize(os);
  _links.write(os);
}

template <typename BitVector, typename Container, typename TrieT>
bool LoudsMap<BitVector, Container, TrieT>::findEntry(const char *key, TrieValueType &value) {
  value = 0;
  uint32_t node;
  uint32_t matched_count;
  _followKey(key, node, matched_count);
  if (matched_count != strlen(key))  return false;

  value = getValue(node);
  return value != 0;
}

template <typename BitVector, typename Container, typename TrieT>
bool LoudsMap<BitVector, Container, TrieT>::findEntryLowerBound(const char *pattern, 
    std::string *key, TrieValueType &value) {
  value = 0;
  uint32_t node;
  uint32_t matched_count;
  _followKey(pattern, node, matched_count);
  if (matched_count != strlen(pattern)) {
    const char *suffix = pattern + matched_count;
    uint32_t next_node = findChildLowerBound(node, suffix);
    if (next_node == 0) {
      next_node = generalSibling(node);
    }
    if (next_node == 0) {
      return false;
    }
    node = next_node;
  }
  uint32_t tail_node = leftMostChild(node);
  value = getValue(tail_node);

  if (key) {
    computeKey(*key, tail_node);
  }
  return true;
}


template <typename BitVector, typename Container, typename TrieT>
void LoudsMap<BitVector, Container, TrieT>::computeKey(string &key, uint32_t node) {
  uint32_t child = node;
  uint32_t parent;
  uint8_t ch;
  while (1) {
    string label;
    computeLabel(child, label);
    key = label + key;

    _trie.parent(child, parent, ch);
    key = (char)ch + key;

    if (parent == root())  break;
    child = parent;
  }
}

template <typename BitVector, typename Container, typename TrieT>
void LoudsMap<BitVector, Container, TrieT>::_followKey(const char *key, uint32_t &node, uint32_t &matched_count) {
  matched_count = 0;
  node = root();
  int key_len = strlen(key);
  while (matched_count < key_len) {
    uint32_t next_node;
    uint32_t matched_count_this;
    findChild(node, key + matched_count, next_node, matched_count_this);
    if (next_node == 0) {
      break;
    }

    node = next_node;
    matched_count += matched_count_this;
  }
}

template <typename BitVector, typename Container, typename TrieT>
void LoudsMap<BitVector, Container, TrieT>::computeLabel(uint32_t node, string &label) {
  uint32_t link = getLink(node);
  if (link == 0)  return;

  assert(_label_trie);
  _label_trie->computePrefix(link, label);
}

template <typename BitVector, typename Container, typename TrieT>
uint32_t LoudsMap<BitVector, Container, TrieT>::getLink(uint32_t node) {
  uint32_t has_links_pos = loudsRank1(node) - 1 - 1;
  if (_has_links[has_links_pos] == 0)  return 0;

  uint32_t links_pos = hasLinksRank1(has_links_pos) - 1;
  uint32_t link = _links[links_pos];
  return link;
}

template <typename BitVector, typename Container, typename TrieT>
TrieValueType LoudsMap<BitVector, Container, TrieT>::getValue(uint32_t node) {
  // -1 turn rank to offset, -1 for super root
  uint32_t is_tails_pos = loudsRank1(node) - 1 - 1;
  if (_is_tails[is_tails_pos] == 0)  return 0;

  uint32_t values_pos = isTailsRank1(is_tails_pos) - 1;
  assert(values_pos < _values.count());
  return _values[values_pos];
}

template <typename BitVector, typename Container, typename TrieT>
void LoudsMap<BitVector, Container, TrieT>::findChild(uint32_t node, const char *key, 
    uint32_t &child, uint32_t &matched_count) {
  matched_count = 0;
  uint8_t ch = key[0];
  uint8_t fetched_ch;
  findChildViaChar(node, ch, child, fetched_ch);
  if (child == 0 || fetched_ch != ch) {
    child = 0;
    return;
  }

  const char *suffix = key + 1;
  string label;
  computeLabel(child, label);
  if (!stringStartsWtih(suffix, label.c_str())) {
    child = 0;
    return;
  }

  matched_count = label.size() + 1;
}

template <typename BitVector, typename Container, typename TrieT>
uint32_t LoudsMap<BitVector, Container, TrieT>::findChildLowerBound(uint32_t node, const char *key) {
  uint32_t child;
  uint8_t ch = key[0];
  uint8_t fetched_ch;
  findChildViaChar(node, ch, child, fetched_ch);
  if (child == 0)  return 0;
  if (fetched_ch > ch)  return child;
  assert(fetched_ch == ch);

  const char *suffix = key + 1;
  string label;
  computeLabel(child, label);
  if (string(suffix) <= label)  return child;

  return sibling(child);
}

template <typename BitVector, typename Container, typename TrieT>
uint32_t LoudsMap<BitVector, Container, TrieT>::leftMostChild(uint32_t node) {
  uint32_t child = node;
  TrieValueType value;
  while (true) {
    value = getValue(child);
    if (value != 0)  break;
    child = childSelect(child, 1);
  }
  return child;
}

#endif
