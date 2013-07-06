#include <cassert>
#include "utils.h"
#include "BTree.h"
#include "BTreeBuilder.h"
#include "BTreeNodeFactory.h"
#include "timer.h"
#include "FCNode.h"
#include "Entry.h"
#include "SBTrie.h"

typedef uint32_t ValueT;

using namespace std;

void loadTokens(const char *fname, vector<string> &tokens) {
  //tokens.push_back("!!!Fuck_You!!!");
  //tokens.push_back("!!!Fuck_You!!!");
  //return;
  loadTokensFromFile(fname, tokens);
  string last = tokens.back();
  if (last.size() == 0)  tokens.erase(tokens.end() - 1);
}

template <class SBTrieT>
void timing(SBTrieT &builder, const vector<string> &tokens) {
  int step = 10000;
  uint32_t value;
  bool found = builder.findEntry("not_found", value);
  assert(!found);
  int count = tokens.size();
  Timer t;
  for (int i = 1; i < count; i+=step) {
    string token = tokens[i];
    bool found = builder.findEntry(token.c_str(), value);
    assert(found);
    assert(value == token.size());

    if (i % 10001 == 0) {
      cout << i << endl;
    }
  }
  count /= step;
  cout << count << endl;
  t.Report(count);
}

void search(const string &idxName, MapFactoryInterface<ValueT> &map, 
    const vector<string> &tokens) {
  BTree<ValueT> bt;
  bt.set_factory(&map);
  bt.load(idxName.c_str());

  timing(bt, tokens);
}

uint32_t filesizeOfFile(const char *filename) {
  struct stat st;

  if (stat(filename, &st) == 0)
    return st.st_size;

  fprintf(stderr, "Cannot determine size of %s: %s\n",
      filename, strerror(errno));

  return -1;
}

void reportSpace(const string &filename, uint32_t original) {
  uint32_t filesize = filesizeOfFile(filename.c_str());
  double ratio = (double)filesize / original;
  cout << "idx size: " << filesize << " ratio: " << ratio << endl;
}

void benchmark(const char *fname) {
  cout << "benchmarking file:" << fname << endl;

  uint32_t filesize_original = filesizeOfFile(fname);
  vector<string> tokens;
  loadTokens(fname, tokens);

  bool loaded;
  cout << "SBTrieUncompressed" << endl;
  SBTrieUncompressed sbtrieu;
  string filename_idx_uncompressed = string(fname) + ".idxu";
  loaded = sbtrieu.load(filename_idx_uncompressed.c_str());
  assert(loaded);
  timing(sbtrieu, tokens);
  //return;

  cout << "SBTrieCompressed" << endl;
  SBTrieCompressed sbtriec;
  string filename_idx = string(fname) + ".idx";
  loaded = sbtriec.load(filename_idx.c_str());
  assert(loaded);
  timing(sbtriec, tokens);
  //return;
  

  MapFactoryInterface<ValueT> *map = NULL;
  string idxName;
  uint32_t filesize = 0;

  cout << "FC" << endl;
  map = new FCNodeFactory<ValueT>;
  idxName = string(fname) + ".fc";
  // build(fname, idxName, *map);
  reportSpace(idxName, filesize_original);
  search(idxName, *map, tokens);
  delete map;
  cout << endl; 

  cout << "BTree" << endl;
  map = new BTreeNodeFactory<ValueT>;
  idxName = string(fname) + ".simple";
  // build(fname, idxName, *map);
  reportSpace(idxName, filesize_original);
  search(idxName, *map, tokens);
  delete map;
  cout << endl;

  cout << "BTreeCompressed" << endl;
  map = new BTreeNodeCompressedFactory<ValueT>;
  idxName = string(fname) + ".snappy";
  // build(fname, idxName, *map);
  reportSpace(idxName, filesize_original);
  search(idxName, *map, tokens);
  delete map;
  cout << endl;

}

int main(int argc, const char *argv[])
{
  assert(argc == 2);
  const char *fname = argv[1];
  benchmark(fname);
}
