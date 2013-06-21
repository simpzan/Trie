#include "Trie.h"
#include "utils.h"

using namespace std;

int main(int argc, const char *argv[])
{
  vector<string> tokens;
  loadTokensFromFile("words.sorted", tokens);

  LinkedTrie trie;
  int count = tokens.size();
  for (int i = 0; i < count; ++i) {
    trie.addEntry(tokens[i].c_str(), i+1);
  }
  getchar();
  return 0;
}
