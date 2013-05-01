#include "BitVectorBuilder.h"

using namespace std;

void BitVectorBuilder::appendRank(bool bit) {
	uint64_t count = _bits.size();
	if (count == 0) {
		_ranks_block.append(0);
		_ranks_unit.append(0);
		return;
	}

	if (count % BIT_PER_BLOCK == 0) {
		_ranks_block.append(_count_one);
		_rank_block = _count_one;
	} else if (count % BIT_PER_UNIT == 0) {
		uint8_t rank_unit = _count_one - _rank_block;
		_ranks_unit.append(rank_unit);
	}
}

void BitVectorBuilder::append(bool bit) {
	this->appendRank(bit);
	_bits.push_back(bit);
	if (bit)  ++_count_one;
}

void BitVectorBuilder::convert2ByteVector(Vector<uint8_t> &bytes) {
	uint64_t bitCount = _bits.size();
	uint8_t tmp_byte = 0;
	for (int bi = 0; bi < bitCount; bi++) {
		int shift = bi % BIT_PER_BYTE;
		uint8_t mask = 1 << shift;
		if (_bits[bi]) {
			tmp_byte |= mask;
		}

		if (bi % BIT_PER_BYTE == BIT_PER_BYTE - 1) {
			bytes.append(tmp_byte);
			tmp_byte = 0;
		}
	}

	if (bitCount % BIT_PER_BYTE != 0)  bytes.append(tmp_byte);	
}

void BitVectorBuilder::write(ostream &os) {
	uint64_t bitCount = _bits.size();
	os.write((char *)&bitCount, sizeof(uint64_t));

	Vector<uint8_t> bytes;
	convert2ByteVector(bytes);
	bytes.write(os);

	_ranks_block.write(os);
	_ranks_unit.write(os);
}

void BitVectorBuilder::display(ostream &os) {
	uint64_t count = _bits.size();
	cout << "bit count: " << count << endl;
	for (int i = 0; i < count; i++) {
		cout << (_bits[i] ? 1 : 0);
		if (i % 8 == 7)  cout << " ";
	}
	cout << endl;
}

uint64_t BitVectorBuilder::size() {
	uint64_t size = (_bits.size() + 7) / 8;
	size += sizeof(uint64_t);

	size += _ranks_block.size();
	size += _ranks_unit.size();
	return size;
}

void BitVectorBuilder::clear() {
	_bits.clear();
	_ranks_block.clear();
	_ranks_unit.clear();
	_count_one = 0;
	_rank_block = 0;
}
