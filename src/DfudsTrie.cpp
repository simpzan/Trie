#include "DfudsTrie.h"
#include <cstring>

using namespace std;

void DfudsTrieLCP::read(istream &is) {
  DfudsTrie::read(is);
  _offsets.read(is);

  //display();
}

uint64_t DfudsTrieLCP::findLCP(const char *key, uint8_t *lcp) const {
  uint64_t offset = 1;
  if (lcp && *lcp) {
    offset = _offsets[*lcp - 1];
    key += *lcp;
  }
  int key_len = 0;
  bool found = _followKey(key, offset, &key_len);
  if (lcp)  *lcp += key_len;
  if (!found)  return 0;

  return _keyRank(offset);
}

uint64_t DfudsTrieLCP::rightNearFindLCP(const char *key, uint8_t *lcp) const {
  uint64_t offset = 1;
  if (lcp && *lcp) {
    offset = _offsets[*lcp - 1];
    key += *lcp;
  }
  int keyLen = 0;
  int depth = 0;
  bool found = _followKey(key, offset, &keyLen, &depth);
  if (!found) {
    uint8_t ch = key[keyLen];
    uint8_t rank = _childLowerBound(offset, ch);
    if (rank != 0) {
      offset = _childSelect(offset, rank);
    }
    else {
      offset = _generalizedSibling(offset); 
      keyLen = depth;
    }
    if (offset == 0) {
      return 0;
    } 
  }
  if (lcp)  *lcp += keyLen;

  uint64_t rank = _keyRank(offset - 1) + 1;
  return rank;
}

uint64_t DfudsTrie::findLCP(const char *key, uint8_t *lcp) const {
  return find(key);
}

uint64_t DfudsTrie::rightNearFindLCP(const char *key, uint8_t *lcp) const {
  return rightNearFind(key);
}

uint64_t DfudsTrie::find(const char *key) const {
  uint64_t offset = 1;
  bool found = _followKey(key, offset, NULL);
  if (!found)  return 0;

  return _keyRank(offset);
}

uint64_t DfudsTrie::rightNearFind(const char *key) const {
  uint64_t offset = 1;
  int keyLen = 0;
  bool found = _followKey(key, offset, &keyLen);
  if (!found) {
    uint8_t ch = key[keyLen];
    uint8_t rank = _childLowerBound(offset, ch);
    if (rank != 0) {
      offset = _childSelect(offset, rank);
    }
    else {
      offset = _generalizedSibling(offset); 
    }
    if (offset == 0) {
      return 0;
    } 
  }

  uint64_t rank = _keyRank(offset - 1) + 1;
  return rank;
}

void DfudsTrie::select(uint64_t rank, string &key) const {
  assert(0 < rank && rank <= _is_keys.rank1(_is_keys.count() - 1));
  key.clear();
  // + 1 to turn offset to rank, -1 to move to the node before.
  uint64_t nodeOffset = _is_keys.select1Naive(rank) + 1 - 1;
  // + 1 to move to the beginning of next node.
  uint64_t id = _dfuds.select1Naive(nodeOffset) + 1;

  while (id != 1) {
    uint64_t parent = _parent(id);
    uint8_t label = _label(parent, id);
    key = (char)label + key;
    id = parent;
  }
}


void DfudsTrie::read(istream &is) {
  _dfuds.read(is);
  _labels.read(is);
  _is_keys.read(is);
}

uint32_t DfudsTrie::mmap(const uint8_t *address) {
  const uint8_t *tmp_address = address;
  uint32_t consumed_size = _dfuds.mmap(tmp_address);
  tmp_address += consumed_size;

  consumed_size = _labels.mmap(tmp_address);
  tmp_address += consumed_size;

  consumed_size = _is_keys.mmap(tmp_address);
  tmp_address += consumed_size;

  return tmp_address - address;
}

void DfudsTrie::clear() {
  _dfuds.clear();
  _labels.clear();
  _is_keys.clear();
}

void DfudsTrie::display() const {
  string key;
  select(1, key);
  cout << "first: " << key << endl;
  int count = _is_keys.rank1(_is_keys.count() - 1);
  select(count, key);
  cout << "last: " << key << endl;

/*  ostream &os = cout;*/
  //os << "Dfuds: ";
  //_dfuds.display(os);
  //os << "Labels: ";
  //_labels.display();
  //os << "IsKeys: ";
  //_is_keys.display(os);
  /*os << endl;*/
}

void DfudsTrie::computeOffsets(const string &key, 
    Vector<uint32_t> &offsets) const {
  int key_len = key.size();
  uint32_t offset = 1;
  for (int i = 0; i < key_len; ++i) {
    uint8_t rank = _childRank(offset, key[i]);
    assert(rank);
    offset = _childSelect(offset, rank);
    assert(offset);
    offsets[i] = offset;
  }
}

// id: inout type. 
bool DfudsTrie::_followKey(const char *key, 
    uint64_t &id, int *prefixLen, int *depth) const {
  int keyLen = strlen(key);
  uint64_t offset = id;
  int ki = 0;
  int last = 0;
  for (; ki < keyLen; ki++) {
    uint8_t ch = key[ki];
    uint8_t rank = _childRank(offset, ch);
    if (rank == 0)  break;
    if (!_dfuds.bitAt(offset + rank))  last = ki;

    offset = _childSelect(offset, rank);
  }
  id = offset;
  if (prefixLen)  *prefixLen = ki;
  if (depth) *depth = last;
  return ki == keyLen;
}

uint8_t DfudsTrie::_childLowerBound(uint64_t id, uint8_t ch) const {
  if (_dfuds[id])  return 0;

  uint64_t rankOpen = _dfuds.rank0(id) - 1;
  uint64_t base = rankOpen - 1;

  for (int i = 0; !_dfuds[id+i]; i++) {
    uint8_t label = _labels[base + i];
    if (ch <= label)  return (i + 1);
  }
  return 0;
}

uint8_t DfudsTrie::_childRank(uint64_t id, uint8_t ch) const {
  if (_dfuds[id])  return 0;

  uint64_t rankOpen = _dfuds.rank0(id) - 1;
  uint64_t base = rankOpen - 1;

  for (int i = 0; !_dfuds.bitAt(id+i); ++i) {
    uint8_t label = _labels[base + i];
    if (ch == label)  return (i + 1);
  }
  return 0;
}

uint64_t DfudsTrie::_childSelect(uint64_t id, uint8_t rank) const {
  uint64_t offsetClose = id + _degree(id);
  uint64_t tmpOffset = offsetClose - rank;
  uint64_t nextOffset = _dfuds.findClose(tmpOffset) + 1;
  return nextOffset;
}

uint64_t DfudsTrie::_generalizedSibling(uint64_t id) const {
  uint64_t offset = id;
  while (true) {
    uint8_t degree = _degree(offset);
    if (degree == 0) break;
    offset = _childSelect(offset, degree);
  }
  ++offset;

  if (offset >= _dfuds.count())  return 0;
  return offset;
}

uint8_t DfudsTrie::_degree(uint64_t id) const {
  int offset = id;
  while (!_dfuds[offset])  ++offset;
  uint8_t degree = offset - id;
  return degree;
}

uint8_t DfudsTrie::_label(uint64_t parent, uint64_t child) const {
  uint64_t openOffset = _dfuds.findOpenNaive(child - 1);
  int degree = _degree(parent);
  uint8_t childRank = parent + degree - openOffset;

  // -1 for the head open, -1 to turn rank to offset.
  uint64_t base = _dfuds.rank0(parent) - 1 - 1;
  return _labels[base + childRank - 1];
}

uint64_t DfudsTrie::_parent(uint64_t id) const {
  uint64_t openOffset = _dfuds.findOpenNaive(id - 1);
  uint64_t closeRank = _dfuds.rank1(openOffset);
  if (closeRank == 0)  return 1;
  uint64_t result = _dfuds.select1Naive(closeRank) + 1;
  return result;
}

uint64_t DfudsTrie::_keyRank(uint64_t id) const {
  uint64_t close_offset = id + _degree(id);
  uint64_t key_offset = _dfuds.rank1(close_offset) - 1;
  uint64_t rank = _is_keys.rank1(key_offset);
  return rank;
}
