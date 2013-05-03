#include "DfudsTrieBuilder.h"
#include <map>

using namespace std;

void DfudsTrieBuilder::buildDfuds() {
	_dfuds.append(false);
	root()->traversePreorderly(*this);
}

bool DfudsTrieBuilder::visitNode(TrieNode &aNode) {
	LinkedTrieNode *node = (LinkedTrieNode *)&aNode;
	map<uint8_t, TrieNode *> &children = node->children();
	for(map<uint8_t, TrieNode *>::iterator itr = children.begin();
			itr != children.end();
			++itr) {
		_dfuds.append(false);
		_labels.append(itr->first);
	}
	_dfuds.append(true);

	bool isTerminal = node->getValue() != 0;
	_is_keys.append(isTerminal);
}

void DfudsTrieBuilder::write(ostream &os) {
	buildDfuds();

	_dfuds.write(os);
	_labels.write(os);
	_is_keys.write(os);
}

void DfudsTrieBuilder::clear() {
	Trie::clear();
	_dfuds.clear();
	_labels.clear();
	_is_keys.clear();
}

uint64_t DfudsTrieBuilder::size() {
	int total_nodes = node_count();
	int size_dfuds = sizeof(uint64_t) + total_nodes * 2;
	int size_is_teminal = sizeof(uint64_t) + total_nodes;
	int size_labels = sizeof(uint64_t) + total_nodes - 1;
	return size_dfuds + size_is_teminal + size_labels;
}
