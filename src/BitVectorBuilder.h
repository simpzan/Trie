#ifndef BIT_VECTOR_BUILDER_H
#define BIT_VECTOR_BUILDER_H

#define BIT_PER_BLOCK 256
#define BIT_PER_UNIT 64
#define BIT_PER_BYTE 8
#define UNIT_PER_BLOCK 4

#include <vector>
#include <stdint.h>
#include <iostream>

class BitVectorBuilder {
  public:
	BitVectorBuilder();
	~BitVectorBuilder() {}

	void append(bool bit);
	void write(std::ostream &os);
	void clear() {  _bits.clear();  }

	uint64_t size();
	uint64_t count()  {  return _bits.size();  }

	void display(std::ostream &os);

  private:
	void convert2BitArray(uint64_t *buf);
	void appendRank(bool bit);
	void finish();

  private:
	std::vector<bool> _bits;
	std::vector<uint64_t> _ranks;
	uint64_t _rank;
	uint64_t _count_one;
};

#endif
