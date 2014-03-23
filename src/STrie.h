#ifndef PTRIE_LOUDS_H
#define PTRIE_LOUDS_H

#include "LoudsMap.h"
#include "LoudsTrie.h"

typedef LoudsTrieCompressed LoudsTrieT;
typedef LoudsMapCompressed LoudsMapT;

// typedef LoudsTrieUncompressed LoudsTrieT;
// typedef LoudsMapUncompressed LoudsMapT;

class STrie {
public:
	STrie() {}
	~STrie() {}

	bool load(const char *indexFile);
	bool find(const char *key, uint32_t &value);
  void display();

private:
	LoudsMapT mainTrie;
	LoudsTrieT labelTrie;
};



#endif
