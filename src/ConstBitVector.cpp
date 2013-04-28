#include <cassert>

#include "ConstBitVector.h"
#include "BitVectorBuilder.h"

using namespace std;

ConstBitVector::ConstBitVector():_bitCount(0) {

}

ConstBitVector::~ConstBitVector() {

}

void ConstBitVector::convertByte2BoolVector(uint8_t byte, int bit_count) {
	for (int i = 0; i < bit_count; i++) {
		uint8_t mask = 1 << i;
		bool bit = byte & mask;
		_bits.push_back(bit);
	}
}

void ConstBitVector::convert2BoolVector(uint8_t *buf, uint64_t len) {
	int count = len - 1;
	for (int bi = 0; bi < count; bi++) {
		uint8_t tmp_byte = buf[bi];
		convertByte2BoolVector(tmp_byte, BIT_PER_UNIT);
	}
	uint8_t byte = buf[len-1];
	int bit_count = _bitCount % BIT_PER_UNIT;
	if (bit_count == 0)  bit_count = BIT_PER_UNIT;
	convertByte2BoolVector(byte, bit_count);
}

bool ConstBitVector::read(istream &is) {
	uint64_t bitCount;
	is.read((char *)&bitCount, sizeof(uint64_t));
	_bitCount = bitCount;

	uint64_t byteCount = (bitCount + BIT_PER_UNIT - 1) / BIT_PER_UNIT;
	uint8_t *buf = new uint8_t[byteCount];
	is.read((char *)buf, byteCount);

	convert2BoolVector(buf, byteCount);

	delete buf;
}

void ConstBitVector::clear() {
	_bits.clear();
	_bitCount = 0;
}

bool ConstBitVector::operator[](uint64_t offset) {
	assert(offset < _bitCount);
	return _bits[offset];
}

uint64_t ConstBitVector::rank1(uint64_t offset) {
	assert(offset < _bits.size());

	uint64_t count = 0;
	for (int i = 0; i <= offset; i++) {
		if (!_bits[i]) {
			continue;
		}
		++count;
	}
	return count;
}

uint64_t ConstBitVector::rank0(uint64_t offset) {
	// +1 to turn offset to count.
	return offset + 1 - rank1(offset);
}

uint64_t ConstBitVector::select1(uint64_t count) {
	return select(count, true);
}

uint64_t ConstBitVector::select0(uint64_t count) {
	return select(count, false);
}

uint64_t ConstBitVector::select(uint64_t count, bool bit) {
	assert(count < _bits.size());

	int tmpCount = 0;
	for (int ai = 0; ai < _bitCount; ai++) {
		if (_bits[ai] != bit) {
			continue;
		}
		++tmpCount;
		if (count == tmpCount) {
			return ai;
		}
	}

	assert(false);
	return 0;
}

uint64_t ConstBitVector::findClose(uint64_t offset) {
	//assert(!_bits[offset]);
	int count = 0;
	for (int i = offset + 1; i < _bitCount; i++) {
		if (_bits[i]) {
			if (count == 0)  return i;
			--count;
		} else {
			++count;
		}
	}

	assert(false);
	return 0;
}

uint64_t ConstBitVector::findOpen(uint64_t offset) {
	assert(_bits[offset]);

	int count = 0;
	for (int i = offset - 1; i >= 0; --i) {
		if (_bits[i]) {
			++count;	
		} else {
			if (count == 0)  return i;
			--count;
		}
	}
	assert(false);
	return 0;
}

uint64_t ConstBitVector::count() {
	return _bits.size();
}

void ConstBitVector::display(ostream &os) {
	uint64_t count = _bits.size();
	cout << "bit count: " << count << endl;
	for (int i = 0; i < count; i++) {
		cout << (_bits[i] ? 1 : 0);
		if (i % 8 == 7)  cout << " ";
	}
	cout << endl;
}

