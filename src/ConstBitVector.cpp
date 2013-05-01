#include <cassert>

#include "ConstBitVector.h"
#include "BitVectorBuilder.h"

using namespace std;

static unsigned char BitsSetTable256[256] = 
{
#   define B2(n) n,     n+1,     n+1,     n+2
#   define B4(n) B2(n), B2(n+1), B2(n+1), B2(n+2)
#   define B6(n) B4(n), B4(n+1), B4(n+1), B4(n+2)
	B6(0), B6(1), B6(1), B6(2)
};
static bool g_table_initialized = false;

void initializeTable() {
	// To initially generate the table algorithmically:
	BitsSetTable256[0] = 0;
	for (int i = 0; i < 256; i++)
	{
		BitsSetTable256[i] = (i & 1) + BitsSetTable256[i / 2];
	}
	g_table_initialized = true;
}

ConstBitVector::ConstBitVector() : _bitCount(0) {
	if (!g_table_initialized)  initializeTable();
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
		convertByte2BoolVector(tmp_byte, BIT_PER_BYTE);
	}
	uint8_t byte = buf[len-1];
	int bit_count = _bitCount % BIT_PER_BYTE;
	if (bit_count == 0)  bit_count = BIT_PER_BYTE;
	convertByte2BoolVector(byte, bit_count);
}

bool ConstBitVector::read(istream &is) {
	uint64_t bitCount;
	is.read((char *)&bitCount, sizeof(uint64_t));
	_bitCount = bitCount;

	_bytes.read(is);
	uint64_t byteCount = (bitCount + BIT_PER_BYTE - 1) / BIT_PER_BYTE;

	convert2BoolVector(_bytes.data(), byteCount);

	_ranks_block.read(is);
	_ranks_unit.read(is);
}

void ConstBitVector::clear() {

	_bits.clear();
	_bitCount = 0;
}

bool ConstBitVector::operator[](uint64_t offset) {
	assert(offset < _bitCount);

	uint64_t byte_id = offset / 8;
	uint8_t byte = _bytes[byte_id];
	uint64_t bit_id = offset % 8;
	return byte >> bit_id & 1;

	return _bits[offset];
}

uint64_t ConstBitVector::rank10(uint64_t offset) {
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

uint8_t countOne(uint8_t byte) {
	uint8_t count = BitsSetTable256[byte];
	return count;
}

uint64_t ConstBitVector::rank1(uint64_t offset) {
	uint64_t block_id = offset / BIT_PER_BLOCK;
	uint64_t count = _ranks_block[block_id]; 	// block

	uint64_t unit_id = offset / BIT_PER_UNIT % UNIT_PER_BLOCK;
	unit_id += block_id * 4;
	count += _ranks_unit[unit_id];  // unit

	uint64_t byte_id = offset / BIT_PER_UNIT / BIT_PER_BYTE;
	uint64_t base = offset / BIT_PER_UNIT * BIT_PER_UNIT / BIT_PER_BYTE;
	for (int i = 0; i < byte_id; ++i) {
		uint8_t byte = _bytes[base + i];
		count += countOne(byte);  // bytes
	}

	uint8_t byte = _bytes[base + byte_id];
	uint8_t bit_id = offset % BIT_PER_BYTE;
	uint8_t mask = 0xFF >> (7 - bit_id);
	byte &= mask;
	count += countOne(byte);  // bits
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

