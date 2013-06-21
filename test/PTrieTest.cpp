#include <gtest/gtest.h>
#include "PTrie.h"
#include "utils.h"
#include "TrieIterator.h"

using namespace std;

TEST(PTrieTest, addFindEntry) {
  PTrie trie;
  const char *key = "test";
  TrieValueType value;

  bool found = trie.findEntry(key, value);
  EXPECT_FALSE(found);
  EXPECT_EQ(0, value);

  trie.addEntry(key, strlen(key));

  found = trie.findEntry(key, value);
  EXPECT_TRUE(found);
  EXPECT_EQ(strlen(key), value);
}

TEST(PTrieTest, clear) {
  PTrie trie;
  const char *key = "test";
  trie.addEntry(key, strlen(key));

  TrieValueType value;
  bool found = trie.findEntry(key, value);
  EXPECT_TRUE(found);
  EXPECT_EQ(strlen(key), value);

  trie.clear();

  found = trie.findEntry(key, value);
  EXPECT_FALSE(found);
  EXPECT_EQ(0, value);
}

TEST(PTrieTest, count) {
  PTrie trie;
  uint32_t count = trie.getNodeCount();
  EXPECT_EQ(1, count);
  count = trie.getKeyCount();
  EXPECT_EQ(0, count);
  
  trie.addEntry("test", 4);
  count = trie.getNodeCount();
  EXPECT_EQ(2, count);
  count = trie.getKeyCount();
  EXPECT_EQ(1, count);
}

TEST(PTrieTest, test) {
  cout << "loading tokens" << endl;
  vector<string> tokens;
  tokens.push_back("tester");
  tokens.push_back("testing");
  tokens.push_back("xyz");
  tokens.push_back("xyzing");
  //loadTokensFromFile("words", tokens);

  PTrie trie;
  int count = tokens.size();
  for (int i = 0; i < count; ++i) {
    string token = tokens[i];
    trie.addEntry(token.c_str(), token.size());
  }

  TrieBfsIterator itr(trie);
  PTrieNode *node;
  while (true) {
    node = (PTrieNode *)itr.next();
    if (node == NULL)  break;

    cout << node->get_id() << endl;
  }
}
