#ifndef SBTRIE_H
#define SBTRIE_H

#include <stdint.h>
#include <string>
#include <iostream>
#include <fstream>
#include <boost/smart_ptr.hpp>
#include "PTrie.h"
#include "SuccinctTrie.h"
#include "LoudsTrie.h"
#include "LoudsMap.h"

template <class LoudsMapT, class LoudsTrieT>
class SBTrie {
 public:

  SBTrie() : _root(new LoudsMapT), _leafNode(new LoudsMapT), 
  _label_trie(new LoudsTrieT) {
    _root->set_label_trie(_label_trie.get());
    _leafNode->set_label_trie(_label_trie.get());
  }
  ~SBTrie() {}

  void build(const char *data_source_filename, const char *idx_filename);
  bool load(const char *idx_filename);

  bool findEntry(const char *key, TrieValueType &value);

 private:
  void updateLinksForLeafNodes(const vector<uint32_t> nodeIds, istream &is, ostream &os, 
    vector<uint32_t> &offsets);

  std::fstream _idx;
  boost::scoped_ptr<LoudsMapT> _root;
  boost::scoped_ptr<LoudsMapT> _leafNode;
  boost::scoped_ptr<LoudsTrieT> _label_trie;
  uint32_t _root_offset;
};

#include "SBTrie.hxx"

typedef SBTrie<LoudsMapCompressed, LoudsTrieCompressed> SBTrieCompressed;
typedef SBTrie<LoudsMapUncompressed, LoudsTrieUncompressed> SBTrieUncompressed;

#endif
