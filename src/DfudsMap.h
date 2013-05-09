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
  DfudsMap() : _is_leaf(true) {}
  virtual ~DfudsMap() {}

  virtual bool load(std::istream &is);
  virtual bool find(const char *key, T &value);

  virtual bool is_leaf()  {  return _is_leaf;  }
  virtual void set_is_leaf(bool is_leaf)  {  _is_leaf = is_leaf;  }

  void clear();
  void display(std::ostream &is);

 private:
  DACWrapper _values;
  //Vector<T> _values;
  DfudsTrie _trie;
  bool _is_leaf;
};

#include "DfudsMap.hxx"

#endif
