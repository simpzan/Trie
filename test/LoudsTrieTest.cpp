
#include <gtest/gtest.h>
#include "LoudsTrie.h"
#include "Trie.h"
#include "utils.h"

using namespace std;

TEST(LoudsTrieTest, computePrefix) {
  vector<string> tokens;
  tokens.push_back("test");
  tokens.push_back("xyzing");
  tokens.push_back("testing");

  LoudsTrie louds;
  vector<uint32_t> ids;
  louds.convert(tokens, ids);

  louds.display();
  int count = tokens.size();
  for (int i = 0; i < count; ++i) {
    string label;
    louds.computePrefix(ids[i], label);

    string token = tokens[i];
    EXPECT_EQ(token, label) << endl;
  }
}
