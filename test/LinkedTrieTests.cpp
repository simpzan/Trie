#include <gtest/gtest.h>
#include "TrieNodeInterface.h"
#include "Trie.h"
#include "DfudsTrie.h"

using namespace std;

TEST(LinkedTrieTests, addFindEntry) {
  LinkedTrie node;
	const char *key = "abc";
  TrieValueType value;
	bool found = node.findEntry(key, value);
  EXPECT_FALSE(found);
	EXPECT_EQ(0, value);

	uint64_t expected = 34;
	node.addEntry(key, expected);

	found = node.findEntry(key, value);
  EXPECT_TRUE(found);
	EXPECT_EQ(expected, value);
}

TEST(LinkedTrieTests, clear) {
  LinkedTrie trie;
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

TEST(LinkedTrieTests, counts) {
  LinkedTrie trie;
  uint32_t count = trie.getNodeCount();
  EXPECT_EQ(1, count);
  count = trie.getKeyCount();
  EXPECT_EQ(0, count);
  
  trie.addEntry("test", 4);
  count = trie.getNodeCount();
  EXPECT_EQ(5, count);
  count = trie.getKeyCount();
  EXPECT_EQ(1, count);
}

