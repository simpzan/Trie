#include "PTrieLouds.h"

using namespace std;

bool PTrieLouds::load(const char *indexFile) {
	ifstream is(indexFile);
	assert(is.good());

	labelTrie.load(is);
	// cout << "load:" << is.tellg() << endl;

	mainTrie.load(is);
	// cout << "load:" << is.tellg() << endl;
	mainTrie.set_label_trie(&labelTrie);

	return true;
}

void PTrieLouds::display() {
    cout << "====label trie:====" << endl;
    labelTrie.display();
    cout << "====main trie:====" << endl;
    mainTrie.display();
    cout << "====end====" << endl;
}

bool PTrieLouds::find(const char *key, uint32_t &value) {
	bool found = mainTrie.findEntry(key, value);
	return found;
}
