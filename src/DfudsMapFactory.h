#ifndef DFUDS_MAP_FACTORY_H
#define DFUDS_MAP_FACTORY_H

#include "Interface.h"
#include "DfudsMap.h"
#include "DfudsMapBuilder.h"

template <typename T>
class DfudsMapFactory : public MapFactory<T> {
  public:
	DfudsMapFactory() {}
	virtual ~DfudsMapFactory() {}

	virtual MapInterface<T> *newMap();
	virtual MapBuilderInterface<T> *newMapBuilder();
};

template <typename T>
inline MapInterface<T> *DfudsMapFactory<T>::newMap() {
	DfudsMap<T> *map = new DfudsMap<T>;
	this->_nodes.push_back(map);
	return map;
}

template <typename T>
inline MapBuilderInterface<T> *DfudsMapFactory<T>::newMapBuilder() {
	DfudsMapBuilder<T> *builder = new DfudsMapBuilder<T>;
	this->_builders.push_back(builder);
	return builder;
}

template <typename T>
class DfudsMapLCPFactory : public MapFactory<T> {
  public:
  DfudsMapLCPFactory() {}
  virtual ~DfudsMapLCPFactory() {}

  virtual MapInterface<T> *newMap() {
    DfudsMapLCP<T> *map = new DfudsMapLCP<T>;
    this->_nodes.push_back(map);
    return map;
  }

  virtual MapBuilderInterface<T> *newMapBuilder() {
    DfudsMapLCPBuilder<T> *builder = new DfudsMapLCPBuilder<T>;
    this->_builders.push_back(builder);
    return builder;
  }

  private:
  
};

#endif
