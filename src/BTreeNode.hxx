#include "BTreeNode.h"

#include <cstring>
#include <cassert>
#include <iostream>

#define KEY_SIZE_MAX 256

using namespace std;

template <typename T>
int BTreeNode<T>::lowerBound(const char *key) {
  int high = _keys.count() - 1;
  int low = 0;
  while (low <= high) {
    int mid = (high + low) >> 1;
    uint32_t offset = _keys[mid];
    const char *aKey = (const char *)_string_buffer.data() + offset;
    int cmp = strcmp(key, aKey);
    if (cmp > 0) {
      low = mid + 1;
    } else {
      high = mid - 1;
    }
  }
  return low;
}

template <typename T>
int BTreeNode<T>::lowerBoundNaive(const char *key) {
  for (int i = 0; i < count(); ++i) {
    const char *aKey = (const char *)_string_buffer.data() + _keys[i];
    if (strcmp(aKey, key) >= 0)  return i;
  }
  return count();
}

template <typename T>
bool BTreeNode<T>::findEntry(const char *key, T &value) {
  value = 0;

  int lowerBound = this->lowerBound(key);
  if (lowerBound == _keys.count()) {
    return false;
  }

  if (_isLeafNode) {
    uint32_t offset = _keys[lowerBound];
    const char *aKey = (const char *)_string_buffer.data() + offset;
    if (strcasecmp(aKey, key)!=0) {
      // not equal case-insensitively, search failed.
      return false;
    }
  }
  value = _values[lowerBound];
  return true;
}

template <typename T>
bool BTreeNode<T>::find(const char *key, T &value) {
  return findEntry(key, value);
}

template <typename T>
bool BTreeNode<T>::load(fstream &is, uint64_t offset) {
  is.seekg(offset);
  return load(is);
}

template <typename T>
bool BTreeNode<T>::load(istream &is) {
   is.read((char *)&_isLeafNode, 1);
  _values.read(is);
  _string_buffer.read(is);
  _keys.read(is);
  return true;
}

template <typename T>
bool BTreeNode<T>::mmap(const uint8_t *address) {
  const uint8_t *tmp_address = address;
  _isLeafNode = *tmp_address;
  ++tmp_address;
  uint32_t consumed_size = _values.mmap(tmp_address);
  tmp_address += consumed_size;
  consumed_size = _string_buffer.mmap(tmp_address);
  tmp_address += consumed_size;
  consumed_size = _keys.mmap(tmp_address);
  return true;
}

template <typename T>
void BTreeNode<T>::display() {
}
