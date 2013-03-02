#include <iostream>
#include <fstream>
#include <cstdio>
#include "ArrayTrie.h"


using namespace std;

int main(int argc, const char **argv) {

	if (argc < 2) {
		cout << "Usage: " << argv[0] << " wordlist" << endl;
	}

	ifstream is(argv[1]);

	ArrayTrie at;

	char word[NODESIZE];
	uint64_t count = 0;
	while (is.getline(word, NODESIZE)) {
		++count;
		at.addEntry(word, count);

		if (count%10000 == 0) {
			cout << count << " size: " << at.sizeInMegaByte() << endl;
			//getchar();
		}
	}
	cout << "size: " << at.sizeInMegaByte() << endl;
	getchar();

	is.close();

	return 0;
}
