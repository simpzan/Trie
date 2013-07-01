
#include <cassert>
#include <deque>
#include <string>
#include <queue>
#include "EntryFileReader.h"
#include "DfudsMap.h"

using std::ios;

template <typename T>
void BTree<T>::display() const {
  std::cout << "hello" << std::endl;
}

template <typename T>
bool BTree<T>::load(const char *idx_filename) {
  const char *idxName = idx_filename;

  _idxStream.open(idxName, ios::in | ios::binary);
  assert(_idxStream.is_open());

  // load the root node.
  uint32_t rootOffset;
  _idxStream.read((char*)&rootOffset, sizeof(rootOffset));

  _root = _factory->newMap();	
  _idxStream.seekg(rootOffset);
  bool loadresult = _root->load(_idxStream);
  assert(loadresult);

  _bufNode = _factory->newMap();
  return true;
}

template <typename T>
bool BTree<T>::findEntry(const char *key, T &value) {
  if (strlen(key) == 0) {
    value = 0;
    return false;
  }

  T offset;
  uint8_t lcp = 0;
  _root->lowerBoundWithLCP(key, offset, &lcp);
  //cout << "lcp:" << (int)lcp << "\t";
  //_root->lowerBound(key, offset);
  if (offset == 0)  return false;

  assert(!_root->is_leaf());
  _idxStream.seekg(offset);
  _bufNode->load(_idxStream);

  while (!_bufNode->is_leaf()) {
    //_bufNode->lowerBound(key, offset);
    _bufNode->lowerBoundWithLCP(key, offset, &lcp);
  //cout  << (int)lcp << "\t";
    assert(offset);
    _idxStream.seekg(offset);
    _bufNode->load(_idxStream);
  }

  bool found = _bufNode->findWithLCP(key, offset, &lcp);
  //cout << (int)lcp << endl << endl;
  //_bufNode->find(key, offset);
  value = offset;
  return found;
}
