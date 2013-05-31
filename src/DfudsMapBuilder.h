#ifndef DFUDS_MAP_BUILDER_H
#define DFUDS_MAP_BUILDER_H

#include <iostream>

#include "DfudsTrieBuilder.h"
#include "Vector.h"
#include "Interface.h"

template <typename T>
class DfudsMapBuilder : public MapBuilderInterface<T>, public TrieVisitorInterface {
 public:
  DfudsMapBuilder() : _is_leaf(false), _trie(new DfudsTrieBuilder) {}
  DfudsMapBuilder(DfudsTrieBuilder *trie) : _is_leaf(false), _trie(trie) {}
  virtual ~DfudsMapBuilder() {  delete _trie;  }

  virtual bool canAddEntry(const char *key, T value);
  virtual void addEntry(const char *key, T value);
  virtual uint32_t save(std::ostream &os);
  virtual void clear();

  virtual void set_is_leaf(bool is_leaf)  {  _is_leaf = is_leaf;  }
  virtual int block_size() {  return _block_size;  }
  virtual void set_block_size(int block_size) {  _block_size = block_size;  }

  virtual bool visitNode(TrieNode &aNode);
  virtual uint32_t sizeWithNewNodeCount(uint32_t count);

 private:
  DfudsTrieBuilder *_trie;
  Vector<T> _values;
  int _block_size;
  bool _is_leaf;
};

template <typename T>
class DfudsMapLCPBuilder : public DfudsMapBuilder<T> {
 public:
  DfudsMapLCPBuilder() : DfudsMapBuilder<T>(new DfudsTrieLCPBuilder) {}
  ~DfudsMapLCPBuilder() {}

 private:

};

#include "DfudsMapBuilder.hxx"

#endif
