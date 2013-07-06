#include "SBTrie.h"
#include "utils.h"

int main(int argc, const char *argv[])
{
  SBTrieCompressed builder;
  //builder.build("words.sorted", "words.sorted.idx");
  builder.load("words.sorted.idx");
  //builder.load("enwiki.idx");
  cout << "build done" << endl;
  return 0;

  vector<string> tokens;
  loadTokensFromFile("words.sorted", tokens);
  uint32_t value;
  int count = tokens.size();
  for (int i = 0; i < count; ++i) {
    string token = tokens[i];
    bool found = builder.findEntry(token.c_str(), value);
    assert(found);
    assert(value == token.size());

    if (i % 10000 == 0)  cout << i << endl;
  }

  return 0;
}
