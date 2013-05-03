#include "DfudsTrie.h"
#include <cstring>

using namespace std;

void DfudsTrie::read(istream &is) {
	_dfuds.read(is);
	_labels.read(is);
	_is_keys.read(is);

	int print = 0;
	if (print)  display(cout);
}

uint64_t DfudsTrie::rightNearFind(const char *key) {
	uint64_t offset = 1;
	int keyLen = 0;
	bool found = followKey(key, false, &offset, &keyLen);
	if (!found) {
		uint8_t ch = key[keyLen];
		uint64_t nextOffset = findChild(offset, ch, true);
		if (nextOffset) {
			offset = nextOffset;
		} else {
			offset = this->rightMost(offset) + 1;
		}
	}
	if (offset == _dfuds.count()) {
		return 0;
	} 

	// -1 to move to the end of previous node.
	uint64_t count = _dfuds.rank1(offset - 1);
    assert(count != 0);
	uint64_t result = 0;
	uint64_t is_terminal_offset = count - 1;
	result = _is_keys.rank1(is_terminal_offset);

	++result;

	int print = 0;
	if (print)  {
		cout << "result: " << result << endl;
		printAllKeys();
	}

	return result;
}

void DfudsTrie::printAllKeys() {
	string aKey;
	int count = _is_keys.rank1(_is_keys.count());
	for (int i = 0; i < count; ++i) {
		select(i, aKey);
		cout << i << ":" << aKey << endl;
	}
}

int DfudsTrie::rightNearLabelRank(uint64_t base, int degree, uint8_t ch) {
	for (int i = 0; i < degree; ++i) {
		uint8_t label = _labels[base + i];
		if (ch <= label)  return i + 1;
	}
	return 0;
}

int DfudsTrie::rightNearLabelRank2(uint64_t base, uint64_t id, uint8_t ch) {
	for (int i = 0; !_dfuds[id+i]; i++) {
		uint8_t label = _labels[base + i];
		if (ch <= label)  return (i + 1);
	}
	return 0;
}

uint64_t DfudsTrie::find(const char *key) {
	uint64_t offset = 1;
	bool found = followKey(key, false, &offset, NULL);
	if (!found)  return 0;

	uint64_t count = _dfuds.rank1(offset - 1);
	uint64_t is_terminal_offset = count - 1;
	// +1 to include node for the offset.
	uint64_t result = _is_keys.rank1(is_terminal_offset + 1);
	return result;
}

bool DfudsTrie::followKey(const char *key, bool near_search,
		uint64_t *id, int *prefixLen) {
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

uint64_t DfudsTrie::findChild(uint64_t offset, uint8_t ch, bool near_search) {
	if (_dfuds[offset])  return 0;
	uint64_t rankClose = _dfuds.rank1(offset);
	uint64_t offsetClose2 = _dfuds.select1(rankClose + 1);
	int degree = offsetClose2 - offset;

	uint64_t rankOpen = _dfuds.rank0(offset) - 1;

	int childRank = near_search ?  rightNearLabelRank2(rankOpen - 1, offset, ch)
		: findLabelRank2(rankOpen - 1, offset, ch);
	if (!childRank)  return 0;

	uint64_t tmpOffset = offsetClose2 - childRank;
	uint64_t nextOffset = _dfuds.findClose(tmpOffset) + 1;
	return nextOffset;
}

int DfudsTrie::findLabelRank2(uint64_t base, uint64_t id, uint8_t ch) {
	for (int i = 0; !_dfuds[id+i]; ++i) {
		uint8_t label = _labels[base+i];
		if (ch == label)  return i+1;
	}
	return 0;
}

int DfudsTrie::findLabelRank(uint64_t base, int degree, uint8_t ch) {
	for (int i = 0; i < degree; ++i) {
		uint8_t label = _labels[base + i];
		if (ch == label)  return (i + 1);
	}
	return 0;
}

void DfudsTrie::display(ostream &os) {
	os << "Dfuds: ";
	_dfuds.display(os);
	os << "Labels: ";
	_labels.display(os);
	os << "IsKeys: ";
	_is_keys.display(os);
	os << endl;
}

void DfudsTrie::clear() {
	_dfuds.clear();
	_labels.clear();
	_is_keys.clear();
}

uint64_t DfudsTrie::parent(uint64_t offset) {
	uint64_t openOffset = _dfuds.findOpenNaive(offset - 1);
	uint64_t closeRank = _dfuds.rank1(openOffset);
	if (closeRank == 0)  return 1;
	uint64_t result = _dfuds.select1(closeRank) + 1;
	return result;
}

uint8_t DfudsTrie::degree(uint64_t offset) {
	if (_dfuds[offset])  return 0;
	uint64_t rankClose = _dfuds.rank1(offset);
	uint64_t offsetClose2 = _dfuds.select1(rankClose + 1);
	int degree = offsetClose2 - offset;
	return degree;
}

uint8_t DfudsTrie::label(uint64_t parent, uint64_t child) {
	uint64_t openOffset = _dfuds.findOpenNaive(child - 1);
	int degree = this->degree(parent);
	uint8_t childRank = parent + degree - openOffset;

	// -1 for the head open, -1 to turn rank to offset.
	uint64_t base = _dfuds.rank0(parent) - 1 - 1;
	return _labels[base + childRank - 1];
}

void DfudsTrie::select(uint64_t rank, string &key) {
	key.clear();
	if (rank == 0)  return;
	uint64_t nodeOffset = _is_keys.select1(rank);
	uint64_t id = _dfuds.select1(nodeOffset) + 1;

	while (id != 1) {
		uint64_t parent = this->parent(id);
		uint8_t label = this->label(parent, id);
		key = (char)label + key;
		id = parent;
	}
}

uint64_t DfudsTrie::rightMost(uint64_t offset) {
	return _dfuds.findCloseNaive(offset - 1);
}
