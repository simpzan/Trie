#ifndef DFUDS_MAP_H
#define DFUDS_MAP_H

#include <vector>
#include <stdint.h>

#include "DfudsTrie.h"
#include "Vector.h"
#include "Interface.h"
#include "DACWrapper.h"

template <typename T>
class DfudsMap : public MapInterface<T> {
 public:
  DfudsMap() : _is_leaf(true), _trie(new DfudsTrie) {}
  DfudsMap(DfudsTrie *trie) : _is_leaf(true), _trie(trie) {}
  virtual ~DfudsMap() {  delete _trie;  }

  virtual bool load(std::istream &is);
  virtual bool mmap(const uint8_t *address);
  virtual bool find(const char *key, T &value);
  virtual bool lowerBound(const char *key, T &value);

  virtual bool is_leaf()  {  return _is_leaf;  }

  void clear();
  void display(std::ostream &is);

 protected:
  DfudsTrie *_trie;
  DACWrapper _values;

 private:
  //Vector<T> _values;
  bool _is_leaf;
};

template <typename T>
class DfudsMapLCP : public DfudsMap<T> {
  public:
  DfudsMapLCP() : DfudsMap<T>(new DfudsTrieLCP) {}
  ~DfudsMapLCP() {}

  virtual bool findWithLCP(const char *key, T &value, uint8_t *lcp);
  virtual bool lowerBoundWithLCP(const char *key, T &value, uint8_t *lcp);

  private:
  
};

#include "DfudsMap.hxx"

#endif
