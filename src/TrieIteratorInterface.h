#ifndef TRIE_ITERATOR_INTERFACE_H
#define TRIE_ITERATOR_INTERFACE_H

#include "TrieNodeInterface.h"

class TrieIteratorInterface {
 public:
  TrieIteratorInterface() {}
  virtual ~TrieIteratorInterface() {}

  virtual TrieNodeInterface *next() = 0;
};

#endif
