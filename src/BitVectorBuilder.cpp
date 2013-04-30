#include "BitVectorBuilder.h"

using namespace std;

BitVectorBuilder::BitVectorBuilder() : _rank(0), _count_one(0) {
}

void BitVectorBuilder::appendRank(bool bit) {
	uint64_t count = _bits.size();
	if (count == 0)  return;

	if (count % BIT_PER_BLOCK == 0) {
		_ranks.push_back(_rank);
		_rank = _count_one << 24;
	} else if (count % BIT_PER_UNIT == 0) {
		uint64_t count_one_in_block = _count_one - (_rank >> 24);
		uint64_t unit_id = count / BIT_PER_UNIT % UNIT_PER_BLOCK;
		_rank |= count_one_in_block << ((unit_id - 1) * 8);
	}
}

void BitVectorBuilder::append(bool bit) {
	this->appendRank(bit);
	_bits.push_back(bit);
	if (bit)  ++_count_one;
}

void BitVectorBuilder::finish() {
	uint64_t count = _bits.size();
	if (count > BIT_PER_BLOCK * _ranks.size()) {
		_ranks.push_back(_rank);
		_rank = _count_one << 24;
	}
}

void BitVectorBuilder::convert2BitArray(uint64_t *buf) {
	uint64_t bitCount = _bits.size();
	uint64_t tmp_byte = 0;
	uint64_t *buf2 = buf;
	for (int bi = 0; bi < bitCount; bi++) {
		int shift = bi % BIT_PER_UNIT;
		uint64_t mask = 0x1ULL << shift;

		if (_bits[bi]) {
			tmp_byte |= mask;
		}

		if (bi % BIT_PER_UNIT == BIT_PER_UNIT - 1) {
			*buf2 = tmp_byte;
			++buf2;
			tmp_byte = 0;
		}
	}
	if (bitCount % BIT_PER_UNIT != 0)  *buf2 = tmp_byte;
}

void BitVectorBuilder::write(ostream &os) {
	this->finish();

	uint64_t bitCount = _bits.size();
	uint64_t unit_count = (bitCount + BIT_PER_UNIT - 1) / BIT_PER_UNIT;
	uint64_t *buf = new uint64_t[unit_count];

	convert2BitArray(buf);
	os.write((char *)&bitCount, sizeof(uint64_t));
	os.write((char *)buf, unit_count * sizeof(uint64_t));

	delete buf;

	uint64_t count = _ranks.size();
	os.write((char *)&count, sizeof(count));
	uint64_t size = sizeof(uint64_t) * count;
	os.write((char *)_ranks.data(), size);
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
	uint64_t bytes = (_bits.size() + 7) / 8;
	bytes += sizeof(uint64_t);
	uint64_t count = _ranks.size();
	uint64_t size_ranks = sizeof(uint64_t) * count;
	bytes += sizeof(count) + size_ranks;
	return bytes;
}
