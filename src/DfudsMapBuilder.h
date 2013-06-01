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

  virtual bool canAddEntry(const char *key, T value, uint32_t limit);
  virtual void addEntry(const char *key, T value) { 
    _trie->addEntry(key, value);  
  }
  virtual void undoAdd() {  _trie->undoAdd();  }
  virtual uint32_t save(std::ostream &os);
  virtual void clear();

  virtual void set_is_leaf(bool is_leaf)  {  _is_leaf = is_leaf;  }

  virtual bool visitNode(TrieNode *aNode);
  virtual uint32_t sizeWithNodeCount(uint32_t count);

 private:
  DfudsTrieBuilder *_trie;
  Vector<T> _values;
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
