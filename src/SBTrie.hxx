#ifndef SBTRIE_HXX
#define SBTRIE_HXX

#include <string>
#include <map>
#include <vector>
#include "SBTrie.h"
#include "EntryFileReader.h"
#include "PTrieNode.h"
#include "Trie.h"
#include "utils.h"
#include "Utilities.h"

using namespace std;

namespace {

uint32_t saveLeafNode(PTrie *trie, ostream &os, LinkedTrie &labels) {
  trie->collectLabels(labels);

  LoudsMapBuilder map;
  map.build(*trie);
  uint32_t offset = os.tellp();
  map.serialize(os);
  trie->clear();

  uint32_t size_node = (uint32_t) os.tellp() - offset;
  cout << offset << " - " << size_node << endl;
  return offset;
}

void buildLeafNodes(const char *filename, ostream &os,
    vector<string> &entries, LinkedTrie &labels) {
  EntryFileReader reader(filename);
  int block_size = 4096;
  PTrie trie;
  string key;
  string lastKey;
  string separator;
  uint64_t value;
  while (reader.nextEntry(key, value)) {
    bool canAdd = trie.canAddEntry(key.c_str(), value, block_size);
    if (!canAdd) {
      saveLeafNode(&trie, os, labels);

      entries.push_back(lastKey);
    }
    trie.addEntry(key.c_str(), value);
    lastKey = key;
  }
  uint32_t offset = saveLeafNode(&trie, os, labels);
  entries.push_back(lastKey);
}

void buildTopTrie(const vector<string> &entries, PTrie &trie) {
  int count = entries.size();
  for (int i = 0; i < count; ++i) {
    trie.addEntry(entries[i].c_str(), i + 1);
  }
}



} // namespace

template <class LoudsMapT, class LoudsTrieT>
void SBTrie<LoudsMapT, LoudsTrieT>::updateLinksForLeafNodes(const vector<uint32_t> nodeIds, 
  istream &is, ostream &os, vector<uint32_t> &offsets) {
  is.seekg(0);
  LoudsMapBuilder builder;
  LoudsMapT map;
  while (!is.eof()) {
    builder.load(is);
    builder.updateLinks(nodeIds);
    map.init(builder);

    uint32_t offset = os.tellp();
    offsets.push_back(offset);
    cout << offset / 1024 / 1024 << endl;

    map.serialize(os);
    is.peek();
  }
}

template <class LoudsMapT, class LoudsTrieT>
void SBTrie<LoudsMapT, LoudsTrieT>::build(const char *data_source_filename, const char *idx_filename) {
  _idx.open(idx_filename, ios::in|ios::out|ios::trunc|ios::binary);
  assert(_idx.good());

  // root offset placeholder.
  _idx.write((char *)&_root_offset, sizeof(uint32_t));

  string tmp_file = string(data_source_filename) + ".tmp";
  fstream io(tmp_file.c_str(), ios::in|ios::out|ios::trunc|ios::binary);
  assert(io.good());

  vector<string> entries;   // entries for root trie.
  LinkedTrie label_trie;
  // build intermediate leaf nodes and store in 'io', get entries for top trie and labels in label trie.
  buildLeafNodes(data_source_filename, io, entries, label_trie);

  // build root trie from entries and collect labels in label trie.
  PTrie trie;
  buildTopTrie(entries, trie);
  trie.collectLabels(label_trie);

  // stablize label trie to get id for each node.
  vector<uint32_t> nodeIds;
  LoudsTrieBuilder builder;
  label_trie.convert(builder, nodeIds);
  // save label trie in the final file.
  _label_trie->init(builder);
  _label_trie->serialize(_idx);

  // update links in the leaf nodes, save leaf node in the final file.
  vector<uint32_t> offsets; // offset of each leaf node.
  updateLinksForLeafNodes(nodeIds, io, _idx, offsets);

  // build and update links and values of the top trie.
  LoudsMapBuilder mapBuilder;
  mapBuilder.build(trie);
  mapBuilder.updateLinks(nodeIds);
  mapBuilder.updateValues(offsets);

  _root_offset = _idx.tellp();
  // save root trie in the final file.
  _root->init(mapBuilder);
  _root->serialize(_idx);

  // update root offset.
  _idx.seekp(0);
  _idx.write((char *)&_root_offset, sizeof(uint32_t));
}

template <class LoudsMapT, class LoudsTrieT>
bool SBTrie<LoudsMapT, LoudsTrieT>::findEntry(const char *key, TrieValueType &value) {
  // search the top trie to find the first key K >= key. get the value V associated with K.
  uint32_t offset;
  string key_found;
  bool found = _root->findEntryLowerBound(key, NULL, offset);
  if (!found)  return false;

  // fetch the leaf node at position V.
  _idx.seekg(offset);
  _leafNode->load(_idx);
  //_leafNode->display();
  // search the leaf node to find the value associated with the key.
  found = _leafNode->findEntry(key, offset);
  if (!found)  return false;

  value = offset;
  return true;
}


template <class LoudsMapT, class LoudsTrieT>
bool SBTrie<LoudsMapT, LoudsTrieT>::load(const char *idx_filename) {
  _idx.open(idx_filename, ios::in|ios::binary);

  assertWithString(_idx.good(), string("open file failed: ") + idx_filename);
  
  _idx.read((char *)&_root_offset, sizeof(uint32_t));

  uint32_t offset = _idx.tellg();
  _label_trie->load(_idx);
  reportLength(_idx, offset, "label trie");
  offset = _idx.tellg();

  _idx.seekg(_root_offset);
  reportLength(_idx, offset, "leaf nodes");

  _root->load(_idx);
  reportLength(_idx, _root_offset, "root");

  cout << "file size: " << _idx.tellg() << endl;

  return true;
}

#endif
