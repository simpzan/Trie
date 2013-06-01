#include "DfudsTrieBuilder.h"
#include "DfudsTrie.h"
#include "utils.h"

using namespace std;

void build(const char *file, const string &idx, vector<string> &tokens) {
  
  DfudsTrieBuilder builder;
  int count = tokens.size();
  for (int i = 0; i < count; ++i) {
    builder.addEntry(tokens[i].c_str(), i);
  }

  ofstream os(idx.c_str());
  builder.write(os);
}

void search(const char *idx, const vector<string> &tokens) {
  ifstream is(idx);
  assert(is.good());
  DfudsTrie trie;
  trie.read(is);

  uint32_t value = 0;
  int count = tokens.size();
  for (int i = 0; i < count; ++i) {
    value = trie.find(tokens[i].c_str());
    assert(value == i);
  }
  cout << "search done" << endl;
}

int main(int argc, const char *argv[])
{
  cout << "test" << endl;
  
  vector<string> tokens;  
  const char *file = "/Volumes/Docs/workspace/testbed/ArrayTrie/words.sorted";
  loadTokensFromFile(file, tokens);

  string idx(file);
  idx += ".dfuds.trie";
  //build(file, idx, tokens);
  
  search(idx.c_str(), tokens);
  
  return 0;
}
