#ifndef MAP_INTERFACE_H
#define MAP_INTERFACE_H

#include <cassert>
#include <vector>
#include <iostream>

class SetInterface {
 public:
  SetInterface() {}
  virtual ~SetInterface() {}

  virtual uint32_t rank(const char *key) = 0;
  virtual bool find(const char *key) = 0;
  virtual bool load(const std::istream &is) = 0;
};

template <typename T>
class MapInterface {
 public:
  MapInterface() {}
  virtual ~MapInterface() {}

  virtual bool find(const char *key, T &value) = 0;
  virtual bool lowerBound(const char *key, T &value) = 0;
  virtual bool load(std::istream &is) = 0;
  virtual bool mmap(const uint8_t *address) {  assert(false);  }
  virtual void display() const {}

  virtual bool is_leaf() = 0;
};

template <typename T>
class MapBuilderInterface {
 public:
  MapBuilderInterface() {}
  virtual ~MapBuilderInterface() {}

  virtual bool canAddEntry(const char *key, T value) = 0;
  virtual void addEntry(const char *key, T value) = 0;
  virtual uint64_t save(std::ostream &os) = 0;
  virtual void clear() = 0;

  virtual void set_is_leaf(bool is_leaf) = 0;

  virtual int block_size() = 0;
  virtual void set_block_size(int block_size) = 0;
};

template <typename T>
class MapFactoryInterface {
 public:
  MapFactoryInterface() {}
  ~MapFactoryInterface() {}

  virtual MapInterface<T>* newMap() = 0;
  virtual MapBuilderInterface<T>* newMapBuilder() = 0;
};

template <typename T>
class MapFactory : public MapFactoryInterface<T> {
 public:
  MapFactory() {}
  virtual ~MapFactory() {
  	for (int i = 0; i < _nodes.size(); ++i) {
			delete _nodes[i];
		}
    for (int i = 0; i < _builders.size(); ++i) {
      delete _builders[i];
    }
  }

 protected:
  std::vector<MapBuilderInterface<T> *> _builders;
  std::vector<MapInterface<T> *> _nodes;
};

#endif
