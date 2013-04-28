#ifndef DFUDS_MAP_FACTORY_H
#define DFUDS_MAP_FACTORY_H

#include "Interface.h"
#include "DfudsMap.h"
#include "DfudsMapBuilder.h"

template <typename T>
class DfudsMapFactory : public MapFactoryInterface<T> {
  public:
	DfudsMapFactory() {}
	virtual ~DfudsMapFactory();

	virtual MapInterface<T> *newMap();
	virtual MapBuilderInterface<T> *newMapBuilder();

  private:
	std::vector<DfudsMap<T> *> _maps;
	std::vector<DfudsMapBuilder<T> *> _builders;
};

template <typename T>
inline DfudsMapFactory<T>::~DfudsMapFactory() {
	for (int i = 0; i < _maps.size(); ++i) {
		delete _maps[i];
	}
	for (int i = 0; i < _builders.size(); ++i) {
		delete _builders[i];
	}
}

template <typename T>
inline MapInterface<T> *DfudsMapFactory<T>::newMap() {
	DfudsMap<T> *map = new DfudsMap<T>;
	_maps.push_back(map);
	return map;
}

template <typename T>
inline MapBuilderInterface<T> *DfudsMapFactory<T>::newMapBuilder() {
	DfudsMapBuilder<T> *builder = new DfudsMapBuilder<T>;
	_builders.push_back(builder);
	return builder;
}

#endif
