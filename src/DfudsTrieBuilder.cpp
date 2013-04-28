#include <map>

#include "DfudsTrieBuilder.h"

using namespace std;

DfudsTrieBuilder::DfudsTrieBuilder() : Trie(new LinkedTrieNode) {

}

DfudsTrieBuilder::~DfudsTrieBuilder() {

}

void displayCollection(const vector<uint8_t> &array) {
	int count = array.size();
	cout << "===content of array: " << count << endl;
	for (int i = 0; i < array.size(); i++) {
		cout << i << ":" << array[i] << " ";
	}
	cout << endl << "===end" << endl;
}

void DfudsTrieBuilder::buildDfuds() {
	_dfuds.append(false);

	root()->traversePreorderly(*this);

#ifndef NDEBUG
	cout << "dfuds:" << _dfuds.count() << endl;
	_dfuds.display(cout);
	cout << "is_terminal:" << _isTerminal.count() << endl;
	displayCollection(_labels);
	cout << "labels:" << _labels.size() << endl;
	_isTerminal.display(cout);
#endif
}

bool DfudsTrieBuilder::visitNode(TrieNode &aNode) {
	LinkedTrieNode *node = (LinkedTrieNode *)&aNode;
	map<uint8_t, TrieNode *> &children = node->children();
	for(map<uint8_t, TrieNode *>::iterator itr = children.begin();
			itr != children.end();
			++itr) {
		_dfuds.append(false);
		_labels.push_back(itr->first);
	}
	_dfuds.append(true);

	bool isTerminal = node->getValue() != 0;
	_isTerminal.append(isTerminal);
}

void DfudsTrieBuilder::writeVector(ostream &os, vector<uint8_t> &array) {
	uint64_t count = array.size();
	os.write((char *)&count, sizeof(uint64_t));
	os.write((char *)array.data(), count);
}

void DfudsTrieBuilder::write(ostream &os) {
	buildDfuds();

	_dfuds.write(os);
	writeVector(os, _labels);
	_isTerminal.write(os);
}

void DfudsTrieBuilder::clear() {
	Trie::clear();
	_dfuds.clear();
	_labels.clear();
	_isTerminal.clear();
}

uint64_t DfudsTrieBuilder::size() {
	int total_nodes = node_count();
	int size_dfuds = sizeof(uint64_t) + total_nodes * 2;
	int size_is_teminal = sizeof(uint64_t) + total_nodes;
	int size_labels = sizeof(uint64_t) + total_nodes - 1;
	return size_dfuds + size_is_teminal + size_labels;
}
