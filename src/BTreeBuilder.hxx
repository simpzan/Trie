#ifndef B_TREE_BUILDER_HXX
#define B_TREE_BUILDER_HXX

#include "BTreeBuilder.h"
#include <string>
#include <queue>
#include <iostream>
#include <cassert>
#include "BTreeBuilder.h"
#include "EntryFileReader.h"
#include "BTreeNode.h"

using std::ios;
using std::cout;
using std::endl;
using std::queue;
using std::string;

static const int g_block_size = 1024*4;

template <typename T>
bool BTreeBuilder<T>::buildLeafNodes(const string &fname, 
		std::queue<Entry> &entries) {
	assert(entries.size() == 0);

	EntryFileReader entryReader(fname);
	string key;
	string lastKey;
	uint64_t value;

	MapBuilderInterface<T> *leafNode = _factory->newMapBuilder();
	leafNode->clear();
	leafNode->set_is_leaf(true);

	while (entryReader.nextEntry(key, value)) {
		bool canAddEntry = leafNode->canAddEntry(key.c_str(), value, _block_size);
		if (!canAddEntry) {
			uint32_t offset = leafNode->save(_idxStream);
			leafNode->clear();
			entries.push(Entry(lastKey, offset));
		}
		leafNode->addEntry(key.c_str(), value);
		lastKey = key;
	}
	// the last leaf node.
	uint32_t offset = leafNode->save(_idxStream);
	entries.push(Entry(lastKey, offset));

  int size = (int)_idxStream.tellp() - sizeof(uint32_t);
  int avg = size / entries.size();
  cout << "average node size:" << avg << endl;
  return true;
}

template <typename T>
bool BTreeBuilder<T>::buildInternalLevel(queue<Entry> &thisLevel, 
		queue<Entry> &nextLevel) {
	assert(thisLevel.size() > 0);
	assert(nextLevel.size() == 0);

	//BTreeNode<uint32_t> *internalNode = new BTreeNode<uint32_t>;
	MapBuilderInterface<T> *internalNode = _factory->newMapBuilder();
	internalNode->clear();
	internalNode->set_is_leaf(false);
	string lastKey;

	while (thisLevel.size()) {
		Entry entry = thisLevel.front();
		thisLevel.pop();

		bool canInsert = internalNode->canAddEntry(entry.key.c_str(), 
				entry.offset, _block_size);
		if (!canInsert) {
			uint32_t offset = internalNode->save(_idxStream);
			internalNode->clear();
			nextLevel.push(Entry(lastKey, offset));
		}
		internalNode->addEntry(entry.key.c_str(), entry.offset);
		lastKey = entry.key;
	}

	uint32_t offset = internalNode->save(_idxStream);
    internalNode->clear();
	nextLevel.push(Entry(lastKey, offset));

	assert(thisLevel.size() == 0);
	assert(nextLevel.size() > 0);
  return true;
}

template <typename T>
bool BTreeBuilder<T>::build(const string &fname) {
	assert(_idxStream.good());

	// write the placeholder of root node offset.
	uint32_t rootOffset = 0;
	_idxStream.seekp(0);
	_idxStream.write((char *)&rootOffset, sizeof(rootOffset));

	// build the bottom level nodes, namely leaf nodes.
	// return the entries for the upper level.
	queue<Entry> thisLevel;
	buildLeafNodes(string(fname), thisLevel);

	// build the internal nodes in level order.
	queue<Entry> nextLevel; // actually it is on the top of |thisLevel|.
	queue<Entry> *entries = &thisLevel;	
	queue<Entry> *outEntries = &nextLevel;
	int level = 1;
	while(entries->size() > 1) {
		level++;
		cout << "level: " << level << "\tentry: " << entries->size() << endl;

		buildInternalLevel(*entries, *outEntries);
		// swap the 2 queues.
		queue<Entry> *tmpEntries = entries;
		entries = outEntries;
		outEntries = tmpEntries;
	}
	// write the true root offset.
	assert(entries->size() == 1);
	rootOffset = entries->front().offset;
	_idxStream.seekp(0);
	_idxStream.write((char *)&rootOffset, sizeof(rootOffset));
	_idxStream.flush();

	return true;
}

template <typename T>
BTreeBuilder<T>::BTreeBuilder(const std::string &fname) 
	: _idxFilename(fname), _block_size(g_block_size) {
	_idxStream.open(_idxFilename.c_str(), 
			ios::in | ios::out | ios::binary | ios::trunc);
	assert(_idxStream.good());	
}

template <typename T>
BTreeBuilder<T>::~BTreeBuilder() {

}

#endif
