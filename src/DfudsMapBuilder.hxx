#ifndef DFUDS_MAP_BUILDER_HXX
#define DFUDS_MAP_BUILDER_HXX

#include "DACWrapper.h"

template <typename T>
bool DfudsMapBuilder<T>::visitNode(TrieNode *node) {
	uint32_t value = node->getValue();
	if (value)  _values.append(value);
}

template <typename T>
uint32_t DfudsMapBuilder<T>::save(std::ostream &os) {
	uint32_t offset = os.tellp();
	os.write((char *)&_is_leaf, 1);

	_trie->write(os);
  _trie->traversePreorderly(*this);

  DACWrapper dac(_values.data(), _values.count());
  dac.write(os);
  //_values.write(os);

  return offset;
}

template <typename T>
bool DfudsMapBuilder<T>::canAddEntry(const char *key, T value, uint32_t limit) {
  uint32_t node_count = _trie->nodeCountAfterInsert(key);
  uint32_t size_all = sizeWithNodeCount(node_count);
	return size_all <= limit;
}

template <typename T>
void DfudsMapBuilder<T>::clear() {
	_trie->clear();
	_values.clear();
}

template <typename T>
uint32_t DfudsMapBuilder<T>::sizeWithNodeCount(uint32_t count) {
	uint32_t size_parent = _trie->sizeWithNodeCount(count);

	uint32_t count_values = _trie->value_count() + 1;
	uint32_t size_values = Vector<T>::sizeWithCount(count_values);
	return size_parent + size_values + 1;
}

#endif
