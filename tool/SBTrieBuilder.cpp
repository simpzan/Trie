
#include "SBTrie.h"
#include "utils.h"

void pauseForAWhile() {
  cout << "press any key to continue" << endl;
  getchar();
}

int main(int argc, const char *argv[])
{
  assert(argc == 2);
  pauseForAWhile();
  const char *src = argv[1];
  string fname = src;

  string fname_u = fname + ".idxu";
  SBTrieUncompressed builder_u;
  builder_u.build(src, fname_u.c_str());

  string fname_c = fname + ".idx";
  SBTrieCompressed builder;
  builder.build(src, fname_c.c_str());
  return 0;
}
