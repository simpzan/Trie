#ifndef DFUDS_MAP_BUILDER_H
#define DFUDS_MAP_BUILDER_H

#include <iostream>

#include "DfudsTrieBuilder.h"
#include "Vector.h"
#include "Interface.h"

template <typename T>
class DfudsMapBuilder : public DfudsTrieBuilder, public MapBuilderInterface<T> {
  public:
	DfudsMapBuilder() : _is_leaf(false) {}
	virtual ~DfudsMapBuilder() {}

	virtual bool visitNode(TrieNode &aNode);
	virtual uint64_t size();
	virtual void write(std::ostream &os);

	virtual bool canAddEntry(const char *key, T value);
	virtual void addEntry(const char *key, T value);
	virtual uint64_t save(std::ostream &os);
	virtual void clear();

	virtual int block_size() {  return _block_size;  }
	virtual void set_block_size(int block_size) {  _block_size = block_size;  }

	virtual bool is_leaf()  {  return _is_leaf;  }
	virtual void set_is_leaf(bool is_leaf)  {  _is_leaf = is_leaf;  }

  private:
	Vector<T> _values;
	int _block_size;
	bool _is_leaf;
};

//#include "DfudsMapBuilder.hxx"

#endif
