
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

  LinkedTrie trie;
  vector<TrieNode *> nodes;
  int count = tokens.size();
  for (int i = 0; i < count; ++i) {
    string token = tokens[i];
    TrieNode * node = trie.addKey(token.c_str());
    nodes.push_back(node);

    string label;
    trie.computePrefix(node, label);
    assert(label == token);
  }

  LoudsTrie louds;
  louds.build(trie);

  louds.display();
  for (int i = 0; i < count; ++i) {
    string token = tokens[i];
    TrieNode *node = nodes[i];
    
    string label;
    louds.computePrefix(node->get_id(), label);

    string reversed;
    reverseKey(label.c_str(), reversed);

    EXPECT_EQ(token, reversed) << label << endl;
  }
}
