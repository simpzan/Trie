#include <iostream>
#include <fstream>
#include <cstdio>
#include "ArrayTrie.h"
#include "LinkedTrie.h"
#include "DfudsTrieBuilder.h"
#include "DfudsTrie.h"
#include "utils.h"
#include "timer.h"
#include "Interface.h"

using namespace std;

bool build(const string &source_filename, const string &idx_filename) {
 	ifstream is(source_filename.c_str());
	DfudsTrieBuilder *t = new DfudsTrieBuilder;
	char word[NODESIZE];
	uint64_t count = 0;
	while (is.getline(word, NODESIZE)) {
		++count;
		t->addEntry(word, count);

		if (count%10000 == 0) {
			cout << count << " size: " << endl; //<< t->sizeInByte() << endl;
		}
	}
	ofstream os(idx_filename.c_str());
	t->write(os);
	delete t; 
  return true;
}

void search(const string &idx_filename, const vector<string> &tokens) {
  ifstream is(idx_filename.c_str());
  DfudsTrie trie;
  trie.read(is);
  
  string token;
  uint64_t value;
  int count = tokens.size();

  Timer timer;
  for (int i = 0; i < count; ++i) {
    token = tokens[i];
    value = trie.find(token.c_str());
  }
  timer.Stop();
  cout << "count:" << count << endl;
  cout << "cpu time(s):" << timer.ElapsedTimeCPU() / 1000000
    << " avg(us):" << timer.ElapsedTimeCPU()/count << endl
    << "wall time(s):" << timer.ElapsedTime() / 1000000
    << " avg(us):" << timer.ElapsedTime()/count << endl;
}

int main(int argc, const char **argv) {
	if (argc < 2) {
		cout << "Usage: " << argv[0] << " wordlist" << endl;
    exit(1);
	}
  
  string source_filename(argv[1]);
  string idx_filename = source_filename + ".idx";
  bool result;
//  = build(source_filename, idx_filename);
//  assert(result);
//  cout << "build done" << endl;

  vector<string> tokens;
  result = loadTokensFromFile(argv[1], tokens);
  if (!result) {
    cout << "load token failed" << endl;
    return -1;
  }
  cout << "tokens count:" << tokens.size() << endl;
  cout << tokens[0] << endl;
  search(idx_filename, tokens);

	return 0;
}

