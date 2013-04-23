#include <iostream>
#include <fstream>
#include <cstdio>
#include "ArrayTrie.h"
#include "LinkedTrie.h"

using namespace std;

int main(int argc, const char **argv) {

	if (argc < 2) {
		cout << "Usage: " << argv[0] << " wordlist" << endl;
	}

	ifstream is(argv[1]);

	Trie *t = new ArrayTrie;

	char word[NODESIZE];
	uint64_t count = 0;
	while (is.getline(word, NODESIZE)) {
		++count;
		t->addEntry(word, count);

		if (count%10000 == 0) {
			cout << count << " size: " << endl; //<< t->sizeInByte() << endl;
			//getchar();
		}
	}
	cout << "size: " << t->sizeInMegaByte() << endl;
	getchar();

	is.close();
	delete t;

	return 0;
}
