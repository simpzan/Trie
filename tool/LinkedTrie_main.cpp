#include "Trie.h"
#include "utils.h"

using namespace std;

int main(int argc, const char *argv[])
{
  assert(argc = 2);
  string file = argv[1];
  vector<string> tokens;
  loadTokensFromFile(file, tokens);

  LinkedTrie trie;
  int count = tokens.size();
  for (int i = 0; i < count; ++i) {
    trie.addEntry(tokens[i].c_str(), i+1);
  }
  getchar();
  return 0;
}
