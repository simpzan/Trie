#include <cassert>

#include "ConstBitVector.h"
#include "BitVectorBuilder.h"

using namespace std;

ConstBitVector::ConstBitVector() : _bitCount(0), _units(NULL), _ranks(NULL) {

}

ConstBitVector::~ConstBitVector() {
	if (_units)  delete _units;
	if (_ranks)  delete _ranks;
}

void ConstBitVector::convertByte2BoolVector(uint64_t byte, int bit_count) {
	for (int i = 0; i < bit_count; i++) {
		uint64_t mask = 1ULL << i;
		bool bit = byte & mask;
		_bits.push_back(bit);
	}
}

void ConstBitVector::convert2BoolVector(uint64_t *buf, uint64_t len) {
	int count = len - 1;
	for (int bi = 0; bi < count; bi++) {
		uint64_t tmp_byte = buf[bi];
		convertByte2BoolVector(tmp_byte, BIT_PER_UNIT);
	}
	uint64_t byte = buf[len-1];
	int bit_count = _bitCount % BIT_PER_UNIT;
	if (bit_count == 0)  bit_count = BIT_PER_UNIT;
	convertByte2BoolVector(byte, bit_count);
}

bool ConstBitVector::read(istream &is) {
	uint64_t bitCount;
	is.read((char *)&bitCount, sizeof(uint64_t));
	_bitCount = bitCount;

	uint64_t byteCount = (bitCount + BIT_PER_UNIT - 1) / BIT_PER_UNIT;
	_units = new uint64_t[byteCount];
	is.read((char *)_units, byteCount*sizeof(uint64_t));
	convert2BoolVector(_units, byteCount);

	uint64_t count = 0;
	is.read((char *)&count, sizeof(count));
	_ranks = new uint64_t[count];
	uint64_t size = sizeof(uint64_t) * count;
	is.read((char *)_ranks, size);
}

void ConstBitVector::clear() {
	if (_ranks) {
		delete _ranks;
		_ranks = NULL;
	}
	if (_units) {
		delete _units;
		_units = NULL;
	}
	_bits.clear();
	_bitCount = 0;
}

bool ConstBitVector::operator[](uint64_t offset) {
	assert(offset < _bitCount);
	return _bits[offset];

	uint64_t unit_id = offset / BIT_PER_UNIT;
	uint64_t bit_id = offset % BIT_PER_UNIT;
	return (_units[unit_id] >> bit_id) & 1;
}

uint64_t ConstBitVector::_rank1(uint64_t unit) {
	unit = ((unit & 0xAAAAAAAAAAAAAAAAULL) >> 1)
		+ (unit & 0x5555555555555555ULL);
	unit = ((unit & 0xCCCCCCCCCCCCCCCCULL) >> 2)
		+ (unit & 0x3333333333333333ULL);
	unit = ((unit >> 4) + unit) & 0x0F0F0F0F0F0F0F0FULL;
	unit += unit << 8;
	unit += unit << 16;
	unit += unit << 32;
	return unit;
}

uint64_t ConstBitVector::rank1(uint64_t offset) {
	assert(offset < _bitCount);

	uint64_t rank = _ranks[offset / BIT_PER_BLOCK];
	uint64_t count = rank >> 24;
	uint64_t unit_id = offset / BIT_PER_UNIT % UNIT_PER_BLOCK;
	count += (rank >> ((unit_id - 1) * 8)) & 0xFF;

	uint64_t bit_id = offset % BIT_PER_UNIT;
	uint64_t mask = ~0ULL >> (BIT_PER_UNIT - bit_id - 1);
	
	uint64_t unit = _units[unit_id] & mask;
	uint64_t count_one = _rank1(unit) >> 56;
	return count + count_one;
}

uint64_t ConstBitVector::rank10(uint64_t offset) {
	assert(offset < _bitCount);

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
	assert(count < _bitCount);
	//cerr << "warning: calling select without index." << endl;

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
	//cerr << "warning: calling findOpen without index." << endl;
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

