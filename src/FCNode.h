#ifndef FC_NODE_H
#define FC_NODE_H

#include "Interface.h"
#include "FrontCoder.h"
#include "Vector.h"
#include "ConstVector.h"

template <typename T>
class FCNodeBuilder : public MapBuilderInterface<T> {
 public:
  FCNodeBuilder() {}
  virtual ~FCNodeBuilder() {}

  virtual bool canAddEntry(const char *key, T value, uint32_t limit) {
    uint32_t size = _coder.sizeAfterInsertKey(key) + _values.size() + sizeof(T);
    return size + 1 <= limit;
  }

  virtual void addEntry(const char *key, T value) {
    _coder.add(key);
    _values.append(value);
  }

  virtual void undoAdd() {
    assert(false);
  }

  virtual uint32_t save(std::ostream &os) {
    uint32_t offset = os.tellp();
    os.write((char *)&_isLeafNode, 1);
    _coder.save(os);
    _values.write(os);
    return offset;
  }

  virtual void clear() {
    _coder.clear();
    _values.clear();
  }

  virtual bool is_leaf() { return _isLeafNode; }
  virtual void set_is_leaf(bool isLeafNode) {  _isLeafNode = isLeafNode;  }

  uint32_t count() {  return _values.count();  }

 private:
  FrontCoderBuilder _coder;
  Vector<T> _values;
  bool _isLeafNode;
};

template <typename T>
class FCNode : public MapInterface<T> {
 public:
  FCNode() {}
  virtual ~FCNode() {}

  virtual bool find(const char *key, T &value) {
    assert(_values.count());
    value = 0;
    uint32_t rank;
    bool found = _coder.find(key, rank);
    if (rank == _coder.count()) {
      return false;
    }
    value = _values[rank];
    if (_isLeafNode) {
      return found;
    }
    return true;
  }
  virtual bool lowerBound(const char *key, T &value) {
    value = 0;
    uint32_t rank;
    bool found = _coder.find(key, rank);
    if (rank == _coder.count())  return false;
    value = _values[rank];
    return true;
  }
  virtual bool load(std::istream &is) {
    is.read((char *)&_isLeafNode, 1);
    _coder.load(is);
    _values.read(is);
    return true;
  }
  virtual bool mmap(const uint8_t *address) {
    const uint8_t *tmp_address = address;
    _isLeafNode = *tmp_address;
    ++tmp_address;
    uint32_t consumed_size = _coder.mmap(tmp_address);
    tmp_address += consumed_size;
    _values.mmap(tmp_address);
    return true;
  }

  virtual bool is_leaf() { return _isLeafNode; }
  void display() const {
    std::cout << (_isLeafNode ? "leaf node" : "internal node") << std::endl;
    _coder.display();
    _values.display();
  }
 private:
  bool _isLeafNode;
  FrontCoder _coder;
  ConstVector<T> _values;
};

template <typename T>
class FCNodeFactory : public MapFactory<T> {
 public:
  FCNodeFactory() {}
  ~FCNodeFactory() {}

  virtual MapBuilderInterface<T> *newMapBuilder() {
    MapBuilderInterface<T> *builder = new FCNodeBuilder<T>;
    this->_builders.push_back(builder);
    return builder;
  }

  virtual MapInterface<T> *newMap() {
    MapInterface<T> *node = new FCNode<T>;
    this->_nodes.push_back(node);
    return node;
  }
};


#endif
