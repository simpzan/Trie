#ifndef PTRIE_LOUDS_H
#define PTRIE_LOUDS_H

#include "LoudsMap.h"
#include "LoudsTrie.h"

typedef LoudsTrieCompressed LoudsTrieT;
typedef LoudsMapCompressed LoudsMapT;

//typedef LoudsTrieUncompressed LoudsTrieT;
//typedef LoudsMapUncompressed LoudsMapT;

class PTrieLouds {
public:
	PTrieLouds() {}
	~PTrieLouds() {}

	bool load(const char *indexFile);
	bool find(const char *key, uint32_t &value);

private:
	LoudsMapT mainTrie;
	LoudsTrieT labelTrie;
};



#endif