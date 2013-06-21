
#include <gtest/gtest.h>
#include "LoudsTrie.h"
#include "Trie.h"
#include "utils.h"

using namespace std;

void insertTokens(const vector<string> &tokens, LinkedTrie &trie) {
  int count = tokens.size();
  for (int i = 0; i < count; ++i) {
    trie.insertKey(tokens[i].c_str());
  }
}

TEST(LoudsTrieTest, computePrefix) {
  vector<string> tokens;
  tokens.push_back("test");
  tokens.push_back("xyzing");
  tokens.push_back("testing");

  LinkedTrie trie;
  insertTokens(tokens, trie);

  LoudsTrie louds;
  vector<uint32_t> ids;
  trie.convert(louds, ids);

  louds.display();
  int count = tokens.size();
  for (int i = 0; i < count; ++i) {
    string label;
    louds.computePrefix(ids[i], label);

    string token = tokens[i];
    EXPECT_EQ(token, label) << endl;
  }
}
