#ifndef BTREENODE_H
#define BTREENODE_H

#include <stdint.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <snappy.h>

#include "Interface.h"
#include "ConstVector.h"
#include "Common.h"

template <typename T>
class BTreeNode : public MapInterface<T> {
 public:
  BTreeNode() : _isLeafNode(false) {}
  virtual ~BTreeNode() {}

  // find the KV pair whose K meet key.
  // behavior diff on leaf node and internal node.
  virtual bool findEntry(const char *key, T &value);
  // load node data from file at the offset.
  bool load(std::fstream &is, uint64_t offset);
  virtual bool mmap(const uint8_t *address);

  // display the content of this node to stdout. for debugging.
  void display();
  uint32_t count() {  return _keys.count();  }

  virtual bool is_leaf() { return _isLeafNode; }

  virtual bool load(std::istream &is);
  virtual bool find(const char *key, T &value);

 protected:
  // find the first key equal or greater than the key.
  virtual int lowerBound(const char *key);
  virtual int lowerBoundNaive(const char *key);

 protected:
  ConstVector<T> _values;
  ConstVector<uint8_t> _string_buffer;
  ConstVector<uint32_t> _keys;

  bool _isLeafNode;

  DISALLOW_COPY_AND_ASSIGN(BTreeNode);
};

template <typename T>
class BTreeNodeCompressed : public BTreeNode<T> {
 public:
  BTreeNodeCompressed() {}
  ~BTreeNodeCompressed() {}

  virtual bool load(std::istream &is) {
    uint32_t size = 0;
    is.read((char *)&size, sizeof(size));

    std::vector<uint8_t> input(size);
    is.read((char *)input.data(), size);

    std::string output;
    snappy::Uncompress((char *)input.data(), input.size(), &output);
    std::stringstream ss(output);  
    return BTreeNode<T>::load(ss);
  }
};

#include "BTreeNode.hxx"

#endif
