#ifndef BTREE_NODE_BUILDER_H
#define BTREE_NODE_BUILDER_H

#include <sstream>
#include <snappy.h>
#include "Vector.h"
#include "Interface.h"

template <typename T>
class BTreeNodeBuilder : public MapBuilderInterface<T> {
 public:
  BTreeNodeBuilder() {}
  virtual ~BTreeNodeBuilder() {}

  virtual bool canAddEntry(const char *key, T value);
  virtual void addEntry(const char *key, T value);
  virtual uint64_t save(std::ostream &os);
  virtual void clear();

  virtual bool is_leaf() { return _isLeafNode; }
  virtual void set_is_leaf(bool isLeafNode) {  _isLeafNode = isLeafNode;  }

  virtual int block_size() {  return _block_size;  }
  virtual void set_block_size(int block_size) {  _block_size = block_size;  }

  uint32_t count() {  return _keys.count();  }

 private:
  uint32_t lowerBound(const char *key);
  uint32_t sizeWithNewEntry(const char *key, T value);

 protected:
  Vector<T> _values;
  Vector<uint8_t> _string_buffer;
  Vector<uint32_t> _keys;

  bool _isLeafNode;
  int _block_size;
};

template <typename T>
bool BTreeNodeBuilder<T>::canAddEntry(const char *key, T value) {
  uint32_t size = sizeWithNewEntry(key, value);
  return size <= _block_size;
}

template <typename T>
void BTreeNodeBuilder<T>::clear() {
  _values.clear();
  _string_buffer.clear();
  _keys.clear();
}

template <typename T>
uint32_t BTreeNodeBuilder<T>::lowerBound(const char *key) {
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
uint32_t BTreeNodeBuilder<T>::sizeWithNewEntry(const char *key, T value) {
  uint32_t count_key = count() + 1;
  uint32_t size_values = Vector<T>::sizeWithCount(count_key);
  uint32_t count_char = _string_buffer.size() + strlen(key) + 1;
  uint32_t size_strings = Vector<uint8_t>::sizeWithCount(count_char);
  uint32_t size_keys = Vector<uint32_t>::sizeWithCount(count_key);
  return size_values + size_strings + size_keys + 1;
}

template <typename T>
void BTreeNodeBuilder<T>::addEntry(const char *key, T value) {
  uint32_t lowerBound = this->lowerBound(key);
  uint32_t offset = _string_buffer.count();
  _keys.insert(lowerBound, offset);
  _string_buffer.appendValues((const uint8_t *)key, strlen(key) + 1);
  _values.insert(lowerBound, value);
}

template <typename T>
uint64_t BTreeNodeBuilder<T>::save(std::ostream &os) {
  uint64_t offset = os.tellp();
  os.write((char *)&_isLeafNode, 1);
  _values.write(os);
  _string_buffer.write(os);
  _keys.write(os);
  return offset;
}

template <typename T>
class BTreeNodeCompressedBuilder : public BTreeNodeBuilder<T> {
 public:
  BTreeNodeCompressedBuilder() {}
  ~BTreeNodeCompressedBuilder() {}

  uint64_t save(std::ostream &os) {
    std::stringstream ss;
    BTreeNodeBuilder<T>::save(ss);
    std::string input = ss.str();
    std::string output;
    snappy::Compress(input.c_str(), input.size(), &output);

    uint32_t offset = os.tellp();
    uint32_t size = output.size();
    os.write((char *)&size, sizeof(size));
    os.write(output.c_str(), output.size());
    return offset;
  }
};

#endif
