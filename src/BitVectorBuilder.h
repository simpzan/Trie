#ifndef BIT_VECTOR_BUILDER_H
#define BIT_VECTOR_BUILDER_H

#define BIT_PER_UNIT 8

#include <vector>
#include <stdint.h>
#include <iostream>

class BitVectorBuilder 
{
public:
	BitVectorBuilder() {}
	~BitVectorBuilder() {}

	void append(bool bit);
	void write(std::ostream &os);

	uint64_t size();
	uint64_t count()  {  return _bits.size();  }

	void clear() {  _bits.clear();  }
	void display(std::ostream &os);

private:
	void convert2BitArray(uint8_t *buf);

private:
	std::vector<bool> _bits;
};


#endif
