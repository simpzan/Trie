
using namespace std;

template <typename T>
bool DfudsMapBuilder<T>::visitNode(TrieNode &node) {
	DfudsTrieBuilder::visitNode(node);

	uint64_t value = node.getValue();
	if (value)  _values.push_back(value);
}

template <typename T>
void DfudsMapBuilder<T>::write(ostream &os) {
	os.write((char *)&_is_leaf, 1);

	DfudsTrieBuilder::write(os);

	uint64_t count = _values.size();
	os.write((char *)&count, sizeof(count));

	uint64_t size = count * sizeof(T);
	os.write((char *)_values.data(), size);

	return;
	cout << endl;
	cout << "written values: " << count << endl;
	for (int i = 0; i < count; ++i) {
		cout << _values[i] << endl;
	}
	cout << endl;
}

template <typename T>
bool DfudsMapBuilder<T>::canAddEntry(const char *key, T value) {
	int count_new_node = root()->countNewNodeWillCreatedWhenInsertKey(key);
	int size_dfuds = count_new_node * 2 - 1;
	int size_labels = count_new_node;
	int size_is_terminal = 1;
	int size_value = sizeof(value);

	int size_extra = size_dfuds + size_labels + size_is_terminal + size_value;
	uint64_t size_all = size() + size_extra;

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
uint64_t DfudsMapBuilder<T>::size() {
	uint64_t size = DfudsTrieBuilder::size();
	uint64_t value_count = this->value_count();
	uint64_t size_values = sizeof(uint64_t) + value_count * sizeof(T);
	return size + size_values + 1;
}

template <typename T>
uint64_t DfudsMapBuilder<T>::save(std::ostream &os) {
	uint64_t offset = os.tellp();
	write(os);
	return offset;
}

