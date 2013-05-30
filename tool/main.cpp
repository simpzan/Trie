#include <iostream>
#include <fstream>
#include <cstdio>
#include "ArrayTrie.h"
#include "LinkedTrie.h"
#include "DfudsMapBuilder.h"
#include "DfudsMap.h"

using namespace std;

int main(int argc, const char **argv) {
	if (argc < 2) {
		cout << "Usage: " << argv[0] << " wordlist" << endl;
    exit(1);
	}

	ifstream is(argv[1]);

	DfudsMapBuilder<uint32_t> *t = new DfudsMapBuilder<uint32_t>;

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

  string idx_name = string(argv[1]) + ".idx";
	ofstream os(idx_name.c_str());
	t->save(os);

	is.close();
	delete t;

	return 0;
}
