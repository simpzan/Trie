#ifndef BTREE_H
#define BTREE_H

#include <queue>
#include <string>
#include "BTreeNode.h"
#include "Entry.h"
#include "Interface.h"

template <typename T>
class BTree {

  public:
	BTree() {}
	~BTree() {}

	// find
	bool findEntry(const char *key, T &value);
	bool load(const char *idx_filename);

	MapFactoryInterface<T> *factory() {  return _factory;  }
	void set_factory(MapFactoryInterface<T> *factory) {  _factory = factory;  }
  
  void display() const;
  
  private:
	std::fstream _idxStream;

	MapInterface<T> *_bufNode;
	MapInterface<T> *_root;
	MapFactoryInterface<T> *_factory;
};

#include "BTree.hxx"

#endif
