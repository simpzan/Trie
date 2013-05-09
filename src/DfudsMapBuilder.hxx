#include "DACWrapper.h"

template <typename T>
bool DfudsMapBuilder<T>::visitNode(TrieNode &node) {
	DfudsTrieBuilder::visitNode(node);
	uint64_t value = node.getValue();
	if (value)  _values.append(value);
}

template <typename T>
void DfudsMapBuilder<T>::write(std::ostream &os) {
	os.write((char *)&_is_leaf, 1);

	DfudsTrieBuilder::write(os);
	//_values.write(os);
  DACWrapper dac(_values.data(), _values.count());
  dac.write(os);
}

template <typename T>
bool DfudsMapBuilder<T>::canAddEntry(const char *key, T value) {
  // new generated size + existing size.
	int count_new_node = root()->countNewNodeWillCreatedWhenInsertKey(key);

	uint64_t size_all = sizeWithNewNodeCount(count_new_node);
  
  using namespace std;

  //std::cout << "new node:" << count_new_node << std::endl;
  //cout << "size_all:" << size_all << endl;
  //cout << "block_size:" << _block_size << std::endl;
	return size_all <= _block_size;
}

template <typename T>
void DfudsMapBuilder<T>::addEntry(const char *key, T value) {
	Trie::addEntry(key, value);
}

template <typename T>
void DfudsMapBuilder<T>::clear() {
	DfudsTrieBuilder::clear();
	_values.clear();
}

template <typename T>
uint64_t DfudsMapBuilder<T>::sizeWithNewNodeCount(uint32_t count) {
	uint64_t size_parent = DfudsTrieBuilder::sizeWithNewNodeCount(count);
	uint64_t count_values = this->value_count() + 1;
	uint64_t size_values = Vector<T>::sizeWithCount(count_values);
	return size_parent + size_values + 1;
}

template <typename T>
uint64_t DfudsMapBuilder<T>::save(std::ostream &os) {
	uint64_t offset = os.tellp();
	write(os);
	return offset;
}

