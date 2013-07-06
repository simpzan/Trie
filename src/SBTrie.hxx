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

void reportProgress(uint32_t offset) {
  uint32_t unit = 1024 * 1024;
  uint32_t block = offset / unit;
  static uint32_t block_last = 0;
  if (block == block_last)  return;

  block_last = block;
  cout << block << endl;
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

    reportProgress(offset);

    map.serialize(os);
    is.peek();
  }
}

template <class LoudsMapT, class LoudsTrieT>
void SBTrie<LoudsMapT, LoudsTrieT>::build(const char *data_source_filename, 
    const char *idx_filename) {
  _idx.open(idx_filename, ios::in|ios::out|ios::trunc|ios::binary);
  assert(_idx.good());
  _idx.write((char *)&_root_offset, sizeof(uint32_t));

  string tmp_file = string(data_source_filename) + ".tmp";
  fstream tmp_stream(tmp_file.c_str(), ios::in|ios::out|ios::trunc|ios::binary);
  assert(tmp_stream.good());

  // build leaf level to get a set of string labels L and a map KVs.
  cout << "building leaf nodes" << endl;
  vector<string> entries;
  LinkedTrie label_trie;
  buildLeafNodes(data_source_filename, tmp_stream, entries, label_trie);

  // build the top trie using the map of KVs. get labels and merge with previouds labels.
  cout << "root node" << endl;
  PTrie top_trie;
  buildTopTrie(entries, top_trie);
  top_trie.collectLabels(label_trie);

  cout << "building and writing label trie" << endl;
  // convert labels to louds trie and ids.
  vector<uint32_t> nodeIds;
  LoudsTrieBuilder builder;
  label_trie.convert(builder, nodeIds);
  _label_trie->init(builder);
  _label_trie->serialize(_idx);

  // update link of the leaf nodes.
  cout << "updating leaf nodes" << endl;
  vector<uint32_t> offsets;
  updateLinksForLeafNodes(nodeIds, tmp_stream, _idx, offsets);

  // build and update link of the top trie.
  cout << "converting root node to louds and writing" << endl;
  LoudsMapBuilder mapBuilder;
  mapBuilder.build(top_trie);
  mapBuilder.updateLinks(nodeIds);
  mapBuilder.updateValues(offsets);
  _root->init(mapBuilder);

  _root_offset = _idx.tellp();
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
  bool found = _root->findEntryLowerBound(key, &key_found, offset);
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
  assert(_idx.good());
  
  _idx.read((char *)&_root_offset, sizeof(uint32_t));
  bool loaded = _label_trie->load(_idx);
  if (!loaded)  return false;

  _idx.seekg(_root_offset);
  loaded = _root->load(_idx);
  //_root->display();
  return loaded;
}

template <class LoudsMapT, class LoudsTrieT>
void SBTrie<LoudsMapT, LoudsTrieT>::compress(const char *idx_filename_new) {

  ofstream stream_new(idx_filename_new);
  assert(stream_new.good());

  uint32_t offset_root = 0;
  stream_new.write((char *)&offset_root, sizeof(uint32_t));
  
  cout << "label trie" << endl;
  _idx.seekg(sizeof(uint32_t));
  _label_trie->load(_idx);

  LoudsTrieCompressed label_trie;
  label_trie.init(*_label_trie);
  label_trie.serialize(stream_new);

  cout << "leaf nodes" << endl;
  map<uint32_t, uint32_t> offset_leaves;
  LoudsMapCompressed node;
  while (true) {
    uint32_t offset = _idx.tellg();
    if (offset == _root_offset)  break;
    offset_leaves[offset] = stream_new.tellp();
    cout << offset / 1024 / 1024 << endl;

    _leafNode->load(_idx);
    node.init(*_leafNode);
    node.serialize(stream_new);
  }

  cout << "root node" << endl;
  offset_root = stream_new.tellp();
  node.init(*_root);
  node.updateValues(offset_leaves);
  node.serialize(stream_new);

  stream_new.seekp(0);
  stream_new.write((char *)&offset_root, sizeof(uint32_t));
}

#endif
