#include <iostream>
#include <fstream>
#include "ArrayTrie.h"

#define WORDMAXSIZE 256

using namespace std;

int main(int argc, const char **argv) {

	if (argc < 2) {
		cout << "Usage: " << argv[0] << " wordlist" << endl;
	}

	ifstream is(argv[1]);

	ArrayTrie at;

	char word[WORDMAXSIZE];
	uint64_t count = 0;
	while (is.getline(word, WORDMAXSIZE)) {
		string wordtmp(word);
		at.addEntry(wordtmp, count);
		++count;
	}

	is.close();

	return 0;
}
