
template <typename T>
bool DfudsMap<T>::load(std::istream &is) {
	clear();
	is.read((char *)&_is_leaf, 1);

	_trie.read(is);
	_values.read(is);
	return true;
}

template <typename T>
bool DfudsMap<T>::find(const char *key, T &value) {
	uint64_t rank = _is_leaf ? _trie.find(key) : _trie.rightNearFind(key);
	if (!rank) {
		value = 0;
	  	return false;
	}

	value = _values[rank-1];
	return true;
}

template <typename T>
void DfudsMap<T>::clear() {
	_trie.clear();
	_values.clear();
}

template <typename T>
void DfudsMap<T>::display(std::ostream &is) {
 /* _trie.display(is);*/
	//int count = _values.size();
  //std::cout << "values: " << count << std::endl;
	//for (int i = 0; i < count; ++i) {
		//is << _values[i] << "\t";
	//}
  /*std::cout << std::endl;*/
}
