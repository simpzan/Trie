#include "SBTrie.h"

int main(int argc, const char *argv[])
{
  assert(argc == 3);
  const char *uncompressed = argv[1];
  const char *compressed = argv[2];
  SBTrieUncompressed trie_uncompressed;
  trie_uncompressed.load(uncompressed);

  trie_uncompressed.compress(compressed);
  return 0;
}
