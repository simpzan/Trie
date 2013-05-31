

template <typename T>
bool DfudsMapLCP<T>::lowerBoundWithLCP(const char *key, T &value, uint8_t *lcp) {
  value = 0;
  uint32_t rank = this->_trie->rightNearFindLCP(key, lcp);
  if (rank == 0)  return false;

  value = this->_values[rank - 1];
  return true;
}

template <typename T>
bool DfudsMapLCP<T>::findWithLCP(const char *key, T &value, uint8_t *lcp) {
  value = 0;
  uint32_t rank = this->_trie->findLCP(key, lcp);
  if (rank == 0)  return false;

  value = this->_values[rank-1];
  return true;
}

template <typename T>
bool DfudsMap<T>::load(std::istream &is) {
  clear();
  is.read((char *)&_is_leaf, 1);

  _trie->read(is);
  _values.read(is);
  return true;
}

template <typename T>
bool DfudsMap<T>::mmap(const uint8_t *address) {
  assert(false); // DACWrapper has not implemented mmap yet.
}

template <typename T>
bool DfudsMap<T>::find(const char *key, T &value) {
  value = 0;
  uint32_t rank = _trie->find(key);
  if (rank == 0)  return false;

  value = _values[rank-1];
  return true;
}

template <typename T>
bool DfudsMap<T>::lowerBound(const char *key, T &value) {
  value = 0;
  uint32_t rank = _trie->rightNearFind(key);
  if (rank == 0)  return false;

  value = _values[rank - 1];
  return true;
}

template <typename T>
void DfudsMap<T>::clear() {
  _trie->clear();
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

