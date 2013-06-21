#ifndef TRIE_ITERATOR_H
#define TRIE_ITERATOR_H

#include <queue>
#include "TrieIteratorInterface.h"
#include "TrieInterface.h"

class TrieBfsIterator : public TrieIteratorInterface {
 public:
  TrieBfsIterator(TrieInterface &trie);
  virtual ~TrieBfsIterator() {}

  virtual TrieNodeInterface *next();

 private:
  TrieInterface &_trie;
  std::queue<TrieNodeInterface *> _nodes;
};

#endif
