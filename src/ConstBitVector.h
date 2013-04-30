#ifndef CONST_BIT_VECTOR_H
#define CONST_BIT_VECTOR_H

#include <iostream>
#include <stdint.h>
#include <vector>

class ConstBitVector 
{
public:
	ConstBitVector();
	~ConstBitVector();

	uint64_t rank1(uint64_t offset);
	uint64_t rank0(uint64_t offset);

	uint64_t select1(uint64_t count);
	uint64_t select0(uint64_t count);

	uint64_t findClose(uint64_t offset);
	uint64_t findOpen(uint64_t offset);

	bool operator[](uint64_t offset);

	bool read(std::istream &is);
	void clear();
	uint64_t count();
	void display(std::ostream &os);

private:
	void convertByte2BoolVector(uint64_t byte, int bit_count);
	void convert2BoolVector(uint64_t *buf, uint64_t len);
	uint64_t select(uint64_t count, bool bit);
	uint64_t _rank1(uint64_t unit);
	uint64_t rank10(uint64_t offset);

private:
	std::vector<bool> _bits;
	uint64_t *_units;
	uint64_t *_ranks;
	uint64_t _bitCount;
	
};

#endif
