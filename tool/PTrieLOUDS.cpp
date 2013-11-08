#include "PTrie.h"
#include <fstream>

#include "LoudsMapBuilder.h"
#include "LoudsTrieBuilder.h"

#include "LoudsMap.h"
#include "LoudsTrie.h"

#include "utils.h"

using namespace std;

void insertEntriesIntoTrie(const vector<string> &tokens, PTrie &trie) {
  for (int i = 0; i < tokens.size(); ++i) {
    trie.addEntry(tokens[i].c_str(), i);
  }
}

void clear(vector<string> &vector) {

}

void buildPTrie(const string &input_filename, PTrie &trie) {
  // read entries into memory
  vector<string> tokens;
  loadTokensFromFile(input_filename, tokens);

  getchar();
  // insert entries into Ptrie
  insertEntriesIntoTrie(tokens, trie);
}

void run(const string &input_filename, const string &output_filename) {
  PTrie trie;
  buildPTrie(input_filename, trie);
  
  getchar();
  // convert Ptrie to 2 tries.
  LinkedTrie label_trie;
  trie.collectLabels(label_trie);
  
  LoudsTrieBuilder trie_builer;
  vector<uint32_t> nodeIds;
  label_trie.convert(trie_builer, nodeIds);
  
  ofstream os(output_filename.c_str());
  assert(os.good());

  LoudsTrieCompressed label_trie_louds;
  label_trie_louds.init(trie_builer);
  label_trie_louds.serialize(os);

  LoudsMapBuilder builder;
  builder.build(trie);
  builder.updateLinks(nodeIds);

  LoudsMapCompressed map;
  map.init(builder);
  map.serialize(os);
}

int main(int argc, const char *argv[])
{
  assert(argc == 2);
  string input = argv[1];
  string output = input + ".PTrieLouds";
  run(input, output); 
  return 0;
}
