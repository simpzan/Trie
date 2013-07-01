#include <cassert>
#include <algorithm>
#include "utils.h"
#include "BTree.h"
#include "BTreeBuilder.h"
#include "BTreeNodeFactory.h"
#include "timer.h"
#include "FCNode.h"
#include "Entry.h"

typedef uint32_t ValueT;

using namespace std;

void loadTokens(const char *fname, vector<string> &tokens) {
  loadTokensFromFile(fname, tokens);
}

void saveTokens(const char *fname, const vector<string> &tokens) {
  ofstream os(fname);
  assert(os.good());
  for (int i = 0; i < tokens.size(); ++i) {
    os << tokens[i] << endl;
  }
}

void build(const char *fname, const string &idxName,
    MapFactoryInterface<ValueT> &factory) {
  BTreeBuilder<ValueT> builder(idxName);
  builder.set_factory(&factory);
  Timer timer;
  builder.build(fname);
  timer.Stop();
  cout << "cpu time:" << timer.ElapsedTimeCPU() 
    << "  wall time:" << timer.ElapsedTime() << endl;
}

void sliceFile(const char *file, int part, string &out) {
  cout << "slicing file:" << file << "into its 1/" << part << endl;
  const string filename(file);
  ifstream is(filename.c_str());
  assert(is.is_open());
  
  stringstream ss;
  ss << filename << "." << part;
  out = ss.str();
  ofstream os(out.c_str());
  assert(os.good());

  string line;
  int i = 0;
  while (!is.eof()) {
    ++i;
    getline(is, line);
    
    if (i % part == 0) {
      os << line << endl;
    }
  }
}

int build(const char *fname) {

  MapFactoryInterface<ValueT> *map = NULL;
  string idxName;

  cout << "BTree" << endl;
  map = new BTreeNodeFactory<ValueT>;
  idxName = string(fname) + ".simple";
  build(fname, idxName, *map);
  delete map;
  cout << endl;

  cout << "FC" << endl;
  map = new FCNodeFactory<ValueT>;
  idxName = string(fname) + ".fc";
  build(fname, idxName, *map);
  delete map;
  cout << endl;

  cout << "BTreeCompressed" << endl;
  map = new BTreeNodeCompressedFactory<ValueT>;
  idxName = string(fname) + ".snappy";
  build(fname, idxName, *map);
  delete map;
  cout << endl;

  return 0;
}

void sortTextFile(const string &filename) {
  cout << "sorting file:" << filename << endl;

  vector<string> tokens;
  loadTokens(filename.c_str(), tokens);
  sort(tokens.begin(), tokens.end());

  stringstream ss;
  ss << filename << "." << 1;
  saveTokens(ss.str().c_str(), tokens);
}

int main(int argc, const char *argv[]) {
  assert(argc == 2);
  string filename = argv[1];
  //sortTextFile(filename);

  stringstream ss;
  ss << filename << "." << 1;
  //filename = ss.str();
  build(filename.c_str());

  return 0;

  int parts[] = {2, 3, 4, 5, 10};
  int count = sizeof(parts) / sizeof(parts[0]);
  for (int i = 0; i < count; ++i) {
    cout << "processing: " << parts[i] << endl;
    string f;
    sliceFile(filename.c_str(), parts[i], f);
    build(f.c_str());
  }

  return 0;
}

