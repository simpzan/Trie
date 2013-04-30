#ifndef BIT_VECTOR_BUILDER_H
#define BIT_VECTOR_BUILDER_H

#define BIT_PER_BYTE 8
#define BIT_PER_UNIT 64
#define UNIT_PER_BLOCK 5
#define BIT_PER_BLOCK BIT_PER_UNIT*UNIT_PER_BLOCK

#include <vector>
#include <stdint.h>
#include <iostream>
#include "Vector.h"

class BitVectorBuilder 
{
public:
	BitVectorBuilder() : _count_one(0), _rank_block(0) {}
	~BitVectorBuilder() {}

	void append(bool bit);
	void write(std::ostream &os);

	uint64_t size();
	uint64_t count()  {  return _bits.size();  }

	void clear() {  _bits.clear();  }
	void display(std::ostream &os);

private:
	void convert2BitArray(uint8_t *buf);
	void appendRank(bool bit);

private:
	std::vector<bool> _bits;
	Vector<uint32_t> _ranks_block;
	Vector<uint8_t> _ranks_unit;
	uint32_t _count_one;
	uint32_t _rank_block; // rank1 of the start of the block.
};


#endif
