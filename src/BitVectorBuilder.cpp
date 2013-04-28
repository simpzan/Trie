#include "BitVectorBuilder.h"

using namespace std;

void BitVectorBuilder::append(bool bit) {
	_bits.push_back(bit);
}

void BitVectorBuilder::convert2BitArray(uint8_t *buf) {
	uint64_t bitCount = _bits.size();
	uint8_t tmp_byte = 0;
	uint8_t *buf2 = buf;
	for (int bi = 0; bi < bitCount; bi++) {
		int shift = bi % BIT_PER_UNIT;
		uint8_t mask = 1 << shift;

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
	uint64_t bitCount = _bits.size();
	uint64_t byteCount = (bitCount + BIT_PER_UNIT - 1) / BIT_PER_UNIT;
	uint8_t *buf = new uint8_t[byteCount];

	convert2BitArray(buf);

	os.write((char *)&bitCount, sizeof(uint64_t));
	os.write((char *)buf, byteCount);

	delete buf;
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
	uint64_t size = _bits.size();
	size += sizeof(uint64_t);
	return size;
}
