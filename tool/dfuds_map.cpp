#include <iostream>
#include <fstream>
#include <cstdio>
#include <algorithm>
#include "utils.h"
#include "timer.h"
#include "Interface.h"
#include "file-map.h"

#include "LinkedTrie.h"

#include "BTreeNodeBuilder.h"
#include "BTreeNode.h"
#include "BTreeNodeFactory.h"

#include "DfudsMapFactory.h"
#include "DfudsMap.h"
#include "DfudsMapBuilder.h"

#include "FCNode.h"

#define NODESIZE 256

using namespace std;
typedef uint32_t ValueT;

bool build(const string &source_filename, const string &idx_filename,
    MapFactoryInterface<ValueT> &factory) {
 	ifstream is(source_filename.c_str());
	MapBuilderInterface<ValueT> *t = factory.newMapBuilder();
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
	t->save(os);
  return true;
}

void randomize_keys(std::vector<std::string> &keys) {
	std::cerr << "randomizing keys..." << std::endl;
	std::random_shuffle(keys.begin(), keys.end());
}

void search2(MapInterface<ValueT> *map, const vector<string> &tokens) {
  string token;
  ValueT value;
  int count = tokens.size();

  //map->find("test", value);
  Timer timer;
  for (int i = 0; i < count; i += 1000) {
    token = tokens[i];
    map->find(token.c_str(), value);
    assert(value = i+1);
  }
  timer.Stop();
  count /= 1000;
  cout << "count:" << count << endl;
  cout << "cpu time(s):" << timer.ElapsedTimeCPU() / 1000000
    << " avg(us):" << timer.ElapsedTimeCPU()/count << endl
    << "wall time(s):" << timer.ElapsedTime() / 1000000
    << " avg(us):" << timer.ElapsedTime()/count << endl;
}

void search(const string &idx_filename, const vector<string> &tokens,
    MapFactoryInterface<ValueT> &factory) {
  cout << "press any key to continue" << endl;
  getchar(); 

  MapInterface<ValueT> *map = factory.newMap();

  cout << "mmap" << endl;
  FileMap filemap;
  filemap.open(idx_filename.c_str());
  const uint8_t *address = (const uint8_t *) filemap.address();
  map->mmap(address);
  search2(map, tokens);
  cout << endl;

  cout << "RAM" << endl;
  ifstream is(idx_filename.c_str());
  assert(is.good());
  map = factory.newMap();
  map->load(is);
  search2(map, tokens);

  cout << "press any key to continue" << endl;
  getchar();
}

int main(int argc, const char **argv) {
  MapFactoryInterface<ValueT> *factory = NULL;
  bool result;

  string source_filename = "words.sorted";
  vector<string> tokens;
  result = loadTokensFromFile(source_filename.c_str(), tokens);
  assert(result);
  cout << "tokens count:" << tokens.size() << endl;
  //randomize_keys(tokens);

  cout << "FC" << endl;
  factory = new FCNodeFactory<ValueT>;
  string idx_filename = source_filename + ".fc";
  //result = build(source_filename, idx_filename, *factory);
  //assert(result);
  cout << "build done" << endl;
  search(idx_filename, tokens, *factory);
  delete factory;
  cout << endl;

  cout << "simple" << endl;
  factory = new BTreeNodeFactory<ValueT>;
  idx_filename = source_filename + ".simple";
  //result = build(source_filename, idx_filename, *factory);
  //assert(result);
  cout << "build done" << endl;
  search(idx_filename, tokens, *factory);
  delete factory;
  cout << endl;

/*  cout << "dfuds" << endl;*/
  //factory = new DfudsMapFactory<ValueT>;
  //idx_filename = source_filename + ".dfuds";
  //result = build(source_filename, idx_filename, *factory);
  //assert(result);
  //cout << "build done" << endl;
  //search(idx_filename, tokens, *factory);
  //delete factory;
  /*cout << endl;*/
	return 0;
}


