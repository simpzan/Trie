#ifndef DFUDS_TRIE_H
#define DFUDS_TRIE_H

#include <vector>
#include "ConstBitVector.h"
#include "Interface.h"

class DfudsTrie {
  public:
	DfudsTrie() {}
	~DfudsTrie() {}

	void read(std::istream &is);
	// return the rank of the key in this trie. 
	// return 0, if the key does not exist.
	uint64_t find(const char *key);
	uint64_t rightNearFind(const char *key);

	void display(std::ostream &os);
	void clear();
	void select(uint64_t rank, std::string &key);

  private:
	bool followKey(const char *key, bool near_search, 
			uint64_t *id, int *prefixLen);
	int findLabelRank(uint64_t base, int degree, uint8_t ch);
	int rightNearLabelRank(uint64_t base, int degree, uint8_t ch);
	void readVector(std::istream &is, std::vector<uint8_t> &array);
	uint64_t findChild(uint64_t offset, uint8_t ch, bool near_search);
	uint64_t parent(uint64_t offset);
	uint8_t degree(uint64_t offset);
	uint8_t label(uint64_t parent, uint64_t child);
	uint64_t rightMost(uint64_t offset);

	ConstBitVector _dfuds;
	std::vector<uint8_t> _labels;
	ConstBitVector _isTerminal;
};

#endif
