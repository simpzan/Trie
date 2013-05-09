#include "DfudsTrie.h"
#include <cstring>

using namespace std;

namespace {

// find the right most node in the subtree rooted at offset.
// there are 2 methods:
// 1, findClose( dfuds.enclose(id) ).
// 2, continuously findClose to search the double close position.
// this function use method 2.
uint64_t rightMost(const ConstBalancedBitVector &dfuds, uint64_t offset) {
  if (dfuds.bitAt(offset))  return offset;

  uint64_t id = offset;
  int i = 0;
  while (!dfuds.bitAt(id)) {
    id = dfuds.findClose(id) + 1;
    ++i;
  }
  if (i > 16) {
    cout << "rightMost iteration:" << i << endl;
  }
  return id;
}

uint8_t degree(const ConstBalancedBitVector &dfuds, uint64_t id) {
  int offset = id;
  while (!dfuds[offset])  ++offset;
  uint8_t degree = offset - id;
  return degree;
}

uint8_t label(const ConstBalancedBitVector &dfuds, 
    const Vector<uint8_t> &labels, uint64_t parent, uint64_t child) {
  uint64_t openOffset = dfuds.findOpenNaive(child - 1);
  int degree = ::degree(dfuds, parent);
  uint8_t childRank = parent + degree - openOffset;

  // -1 for the head open, -1 to turn rank to offset.
  uint64_t base = dfuds.rank0(parent) - 1 - 1;
  return labels[base + childRank - 1];
}

uint64_t parent(const ConstBalancedBitVector &dfuds, uint64_t offset) {
  uint64_t openOffset = dfuds.findOpenNaive(offset - 1);
  uint64_t closeRank = dfuds.rank1(openOffset);
  if (closeRank == 0)  return 1;
  uint64_t result = dfuds.select1Naive(closeRank) + 1;
  return result;
}

int findLabelRank(const ConstBalancedBitVector &dfuds, uint64_t id, 
    const Vector<uint8_t> &labels, uint64_t base, uint8_t ch) {
  for (int i = 0; !dfuds.bitAt(id+i); ++i) {
    uint8_t label = labels[base + i];
    if (ch == label)  return (i + 1);
  }
  return 0;
}

int rightNearLabelRank(const ConstBalancedBitVector &dfuds, uint64_t id, 
    const Vector<uint8_t> &labels, uint64_t base, uint8_t ch) {
  for (int i = 0; !dfuds[id+i]; i++) {
    uint8_t label = labels[base + i];
    if (ch <= label)  return (i + 1);
  }
  return 0;
}

}

uint64_t DfudsTrie::find(const char *key) const {
  uint64_t offset = 1;
  bool found = followKey(key, 0, &offset, NULL);
  if (!found)  return 0;

  uint64_t count = _dfuds.rank1(offset - 1);
  uint64_t is_terminal_offset = count - 1;
  // +1 to include node for the offset.
  uint64_t result = _is_keys.rank1(is_terminal_offset + 1);
  return result;
}

uint64_t DfudsTrie::rightNearFind(const char *key) const {
  uint64_t offset = 1;
  int keyLen = 0;
  bool found = followKey(key, 0, &offset, &keyLen);
  if (!found) {
    uint8_t ch = key[keyLen];
    uint64_t nextOffset = findChild(offset, ch, 1);
    if (nextOffset) {
      offset = nextOffset;
    } else {
      offset = ::rightMost(_dfuds, offset) + 1;
    }
  }
  if (offset == _dfuds.count()) {
    return 0;
  } 

  // -1 to move to the end of previous node.
  uint64_t count = _dfuds.rank1(offset - 1);
  assert(count != 0);
  uint64_t is_terminal_offset = count - 1;
  uint64_t result = _is_keys.rank1(is_terminal_offset);
  ++result;
  return result;
}

void DfudsTrie::select(uint64_t rank, string &key) const {
  assert(0 < rank && rank <= _is_keys.rank1(_is_keys.count() - 1));
  key.clear();
  uint64_t nodeOffset = _is_keys.select1Naive(rank);
  uint64_t id = _dfuds.select1Naive(nodeOffset) + 1;

  while (id != 1) {
    uint64_t parent = ::parent(_dfuds, id);
    uint8_t label = ::label(_dfuds, _labels, parent, id);
    key = (char)label + key;
    id = parent;
  }
}

void DfudsTrie::read(istream &is) {
  _dfuds.read(is);
  _labels.read(is);
  _is_keys.read(is);
}

void DfudsTrie::clear() {
  _dfuds.clear();
  _labels.clear();
  _is_keys.clear();
}

void DfudsTrie::display(ostream &os) const {
  os << "Dfuds: ";
  _dfuds.display(os);
  os << "Labels: ";
  _labels.display(os);
  os << "IsKeys: ";
  _is_keys.display(os);
  os << endl;
}

bool DfudsTrie::followKey(const char *key, int near_search,
    uint64_t *id, int *prefixLen) const {
  int keyLen = strlen(key);
  uint64_t offset = 1;
  int ki = 0;
  for (; ki < keyLen; ki++) {
    uint8_t ch = key[ki];
    uint64_t nextOffset = findChild(offset, ch, near_search);

    if (!nextOffset) {  break;  }
    offset = nextOffset;
  }
  if (id)  *id = offset;
  if (prefixLen)  *prefixLen = ki;
  return ki == keyLen;
}

uint64_t DfudsTrie::findChild(uint64_t offset, uint8_t ch, 
    int near_search) const {
  if (_dfuds[offset])  return 0;

  uint64_t rankOpen = _dfuds.rank0(offset) - 1;
  uint64_t base = rankOpen - 1;

  int childRank;
  if (near_search > 0) {
    childRank = rightNearLabelRank(_dfuds, offset, _labels, base, ch);
  } else {
    childRank = findLabelRank(_dfuds, offset, _labels, base, ch);
  }
  if (!childRank)  return 0;

  uint64_t offsetClose = offset + degree(_dfuds, offset);
  uint64_t tmpOffset = offsetClose - childRank;
  uint64_t nextOffset = _dfuds.findClose(tmpOffset) + 1;
  return nextOffset;
}


