
#include "SBTrie.h"
#include "utils.h"

int main(int argc, const char *argv[])
{
  assert(argc >= 3);
  const char *src = argv[1];
  const char *dst = argv[2];
  if (argc >= 4) {
    SBTrieUncompressed builder;
    builder.build(src, dst);
    return 0;
  }
  SBTrieCompressed builder;
  builder.build(src, dst);
//  builder.load("words.sorted.idx");
  return 0;
}