#include "BitVectorBuilder.h"

using namespace std;

void BitVectorBuilder::appendRank(bool bit) {
	uint64_t count = _bits.size();
	if (count == 0)  return;

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

void BitVectorBuilder::convert2BitArray(uint8_t *buf) {
	uint64_t bitCount = _bits.size();
	uint8_t tmp_byte = 0;
	uint8_t *buf2 = buf;
	for (int bi = 0; bi < bitCount; bi++) {
		int shift = bi % BIT_PER_BYTE;
		uint8_t mask = 1 << shift;

		if (_bits[bi]) {
			tmp_byte |= mask;
		}

		if (bi % BIT_PER_BYTE == BIT_PER_BYTE - 1) {
			*buf2 = tmp_byte;
			++buf2;
			tmp_byte = 0;
		}
	}

	if (bitCount % BIT_PER_BYTE != 0)  *buf2 = tmp_byte;
}

void BitVectorBuilder::write(ostream &os) {
	uint64_t bitCount = _bits.size();
	uint64_t byteCount = (bitCount + BIT_PER_BYTE - 1) / BIT_PER_BYTE;
	uint8_t *buf = new uint8_t[byteCount];
	convert2BitArray(buf);
	os.write((char *)&bitCount, sizeof(uint64_t));
	os.write((char *)buf, byteCount);
	delete buf;

	//_ranks_block.write(os);
	//_ranks_unit.write(os);
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
