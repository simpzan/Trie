#include <cassert>

#include "ConstBitVector.h"
#include "BitVectorBuilder.h"

using namespace std;

namespace {

static unsigned char BitsSetTable256[256] = {
#   define B2(n) n,     n+1,     n+1,     n+2
#   define B4(n) B2(n), B2(n+1), B2(n+1), B2(n+2)
#   define B6(n) B4(n), B4(n+1), B4(n+1), B4(n+2)
  B6(0), B6(1), B6(1), B6(2)
};

static bool g_table_initialized = false;

// To initially generate the table algorithmically:
void initializeTable() {
  BitsSetTable256[0] = 0;
  for (int i = 0; i < 256; i++) {
    BitsSetTable256[i] = (i & 1) + BitsSetTable256[i / 2];
  }
  g_table_initialized = true;
}

uint32_t countOne(uint64_t unit) {
  uint8_t *p = (uint8_t *) &unit;
  return BitsSetTable256[p[0]] + 
    BitsSetTable256[p[1]] + 
    BitsSetTable256[p[2]] +	
    BitsSetTable256[p[3]] +
    BitsSetTable256[p[4]] +
    BitsSetTable256[p[5]] +
    BitsSetTable256[p[6]] +
    BitsSetTable256[p[7]];
}

uint64_t select(const ConstBitVector &bits, uint64_t count, bool bit) {
  uint64_t count_bit = bits.count();
  assert(count < count_bit);
  int tmpCount = 0;
  for (int ai = 0; ai < count_bit; ai++) {
    if (bits[ai] != bit) {
      continue;
    }
    ++tmpCount;
    if (count == tmpCount) {
      return ai;
    }
  }
  assert(false);
}

uint64_t getPreviousUnit(const Vector<uint8_t> &bytes, uint64_t id) {
  uint64_t unit = 0;
  uint8_t *p = (uint8_t *)&unit;

  uint64_t byte_id = id / BIT_PER_BYTE;
  uint64_t count_byte = byte_id % 8;
  uint64_t base = byte_id - count_byte;
  for (int i = 0; i < count_byte+1; ++i) {
    if (base + i >= bytes.size())  break;
    *p = bytes[base + i];
    ++p;
  }

  uint8_t bit_id = id % BIT_PER_BYTE;
  uint8_t shift = (8 - count_byte -1) * BIT_PER_BYTE + 
    (BIT_PER_BYTE - 1 - bit_id);
  uint64_t mask = ~0ULL >> shift;
  unit &= mask;
  return unit;
}

} // namespace


ConstBitVector::ConstBitVector() : _count_bit(0) {
  if (!g_table_initialized)  initializeTable();
}

bool ConstBitVector::operator[](uint64_t offset) const {
  assert(offset < _count_bit);
  uint64_t byte_id = offset / 8;
  uint8_t byte = _bytes[byte_id];
  uint64_t bit_id = offset % 8;
  return byte >> bit_id & 1;
}


uint64_t ConstBitVector::rank1(uint64_t offset) const {
  uint64_t block_id = offset / BIT_PER_BLOCK;
  uint64_t count = _ranks_block[block_id];	// block

  uint64_t unit_id = offset / BIT_PER_UNIT % UNIT_PER_BLOCK;
  if (unit_id != 0) {
    unit_id += block_id * 3; // there are 3 unit_index in a block.
    count += _ranks_unit[unit_id - 1];  // unit
  }

  uint64_t unit = getPreviousUnit(_bytes, offset);
  count += countOne(unit);

  uint64_t expected = rank1Naive(offset);
  assert(expected == count);
  return count;
}

uint64_t ConstBitVector::rank1Naive(uint64_t offset) const {
  assert(offset < count());
  uint64_t count = 0;
  for (int i = 0; i <= offset; i++) {
    if (!bitAt(i)) {
      continue;
    }
    ++count;
  }
  return count;
}

uint64_t ConstBitVector::select1Naive(uint64_t count) const {
  return select(*this, count, true);
}

uint64_t ConstBitVector::select0Naive(uint64_t count) const {
  return select(*this, count, false);
}

void ConstBitVector::read(istream &is) {
  uint64_t bitCount;
  is.read((char *)&bitCount, sizeof(uint64_t));
  _count_bit = bitCount;

  _bytes.read(is);
  _ranks_block.read(is);
  _ranks_unit.read(is);
}

void ConstBitVector::clear() {
  _count_bit = 0;
  _bytes.clear();
  _ranks_block.clear();
  _ranks_unit.clear();
}

void ConstBitVector::display(ostream &os) const {
  uint64_t count = this->count();
  os << "bit count: " << count << endl;
  for (int i = 0; i < count; ++i) {
    if (i % 4 == 0)  os << " ";
    if (i % 8 == 0)  os << " " << i <<":";
    os << (bitAt(i) ? 1 : 0);
  }
  os << endl;

  /* cout << "ranks_block: " << endl;*/
  //os << dec;
  //_ranks_block.display(os);
  //cout << "ranks_unit: " << endl;
  /*_ranks_unit.display(os);*/
}

