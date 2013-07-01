#ifndef BTREEBUILDER_H
#define BTREEBUILDER_H

#include <fstream>
#include <string>
#include <queue>
#include "Entry.h"
#include "Interface.h"

// build a b-tree for string keys with bulk loading technique.
template <typename T>
class BTreeBuilder {
public:
	BTreeBuilder(const std::string &fname);
	~BTreeBuilder();

	bool build(const std::string &fname);

	MapFactoryInterface<T> *factory() {  return _factory;  }
	void set_factory(MapFactoryInterface<T> *factory) {  _factory = factory;  }

	int block_size() {  return _block_size;  }
	void set_block_size(int block_size) {  _block_size = block_size;  }

private:
	bool buildLeafNodes(const std::string &fname, 
			std::queue<Entry> &outEntries);

	bool buildInternalLevel(std::queue<Entry> &inEntries, 
			std::queue<Entry> &outEntries); 
	
private:
	std::string _idxFilename;
	std::fstream _idxStream;
	int _block_size;
	MapFactoryInterface<T> *_factory;
};

#include "BTreeBuilder.hxx"

#endif


