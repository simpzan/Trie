#ifndef BTREE_NODE_FACTORY_H
#define BTREE_NODE_FACTORY_H

#include <vector>
#include "Interface.h"
#include "BTreeNode.h"
#include "BTreeNodeBuilder.h"

template <typename T>
class BTreeNodeFactory : public MapFactory<T> {
  public:
	BTreeNodeFactory() {}
	virtual ~BTreeNodeFactory() {}

	virtual MapInterface<T>* newMap() {
		BTreeNode<T> *node = new BTreeNode<T>;
		this->_nodes.push_back(node);
		return node;
	}

	virtual MapBuilderInterface<T>* newMapBuilder() {
    BTreeNodeBuilder<T> *builder = new BTreeNodeBuilder<T>;
    this->_builders.push_back(builder);
    return builder;
	}

};

template <typename T>
class BTreeNodeCompressedFactory : public MapFactory<T> {
  public:
	BTreeNodeCompressedFactory() {}
	virtual ~BTreeNodeCompressedFactory() {}

	virtual MapInterface<T>* newMap() {
		BTreeNodeCompressed<T> *node = new BTreeNodeCompressed<T>;
		this->_nodes.push_back(node);
		return node;
	}

	virtual MapBuilderInterface<T>* newMapBuilder() {
    BTreeNodeCompressedBuilder<T> *builder = new BTreeNodeCompressedBuilder<T>;
    this->_builders.push_back(builder);
    return builder;
	}

};

#endif
