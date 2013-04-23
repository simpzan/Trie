#include <iostream>
#include <string>

#include "AMTrieNode.h"
#include "Utils.h"

using namespace std;

uint64_t _value;
uint64_t _count = 0;

void displayArray(vector<TrieNode *> &array) {
	cout << "==the content of array: "  << endl;
	for (int i=0; i<array.size(); ++i) {
		cout << i << " : " << array[i] << endl;
	}
	cout << "==end of the content" << endl;
}

int rank1(bitset<256> &map, int ch) {
	int rank = 0;
	for (int i=0; i<=ch; ++i) {
		if (map[i]) {
			++rank;
		}
	}
	return rank;
}

TrieNode *AMTrieNode::getChildNodeWithLabel(uint8_t ch) {


	int isON = _map[ch];
	if (!isON) {
		return NULL;
	}

	int rank = rank1(_map, ch);

	return (*_children)[rank-1];
}

void AMTrieNode::setChildNodeWithLabel(uint8_t ch, TrieNode *node) {

	int isON = _map[ch];
	int rank = rank1(_map, ch);

	if (isON) {
		if ((*_children)[rank-1] != node) {
			delete (*_children)[rank];
		}
		(*_children)[rank-1] = (AMTrieNode *) node;
	} else {
		(*_children).insert((*_children).begin()+rank, (AMTrieNode *)node);

		_map[ch] = 1;
	}

}


