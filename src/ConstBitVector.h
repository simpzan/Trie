#ifndef CONST_BIT_VECTOR_H
#define CONST_BIT_VECTOR_H

#include <iostream>
#include <stdint.h>
#include "Vector.h"

#define BIT_PER_BYTE 8
#define BIT_PER_UNIT 64
#define UNIT_PER_BLOCK 4
#define BIT_PER_BLOCK (BIT_PER_UNIT*UNIT_PER_BLOCK)

typedef enum _BitVectorType {
	kRankIndexOnly,
	kFindCloseIndex,
} BitVectorType;

class ConstBitVector 
{
public:
	ConstBitVector();
	virtual ~ConstBitVector() {}

	bool operator[](uint64_t offset) const;
	bool bitAt(uint64_t offset) const {  return operator[](offset);  }
	uint64_t count() const {  return _count_bit;  }

	uint64_t rank1(uint64_t id) const;
	uint64_t rank1Naive(uint64_t id) const;
	uint64_t rank0(uint64_t id) const {  return id + 1 - rank1(id);  }

	uint64_t excess(uint64_t id) const {  return rank0(id) - rank1(id);  }

	uint64_t select1(uint64_t count) const;
	uint64_t select0(uint64_t count) const;

	uint64_t findOpenNaive(uint64_t id) const;
	uint64_t findCloseNaive(uint64_t id) const;

	virtual void read(std::istream &is);
	virtual void clear();
	virtual void display(std::ostream &os) const;

protected:
	const Vector<uint8_t> &bytes() const {  return _bytes;  }

private:
	uint64_t _count_bit;

	Vector<uint8_t> _bytes;
	Vector<uint32_t> _ranks_block;
	Vector<uint8_t> _ranks_unit;
};

#endif
