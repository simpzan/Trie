#include <gtest/gtest.h>
#include "LoudsMap.h"
#include "LoudsTrie.h"
#include "PTrie.h"
#include "Trie.h"
#include "utils.h"

using namespace std;

namespace {

void addEntries(const vector<string> &tokens, PTrie &trie0) {
  int count = tokens.size();
  for (int i = 0; i < count; ++i) {
    const char *token = tokens[i].c_str();
    trie0.addEntry(token, i + 1);
  }
}

}

TEST(LoudsMapTest, findEntry) {
  vector<string> tokens;
  loadTokensFromFile("block", tokens);

  cout << "build main trie" << endl;
  PTrie trie0;
  addEntries(tokens, trie0);
  vector<string> labels;
  trie0.collectLabels(labels);

  cout << "building label trie" << endl;
  LoudsTrie label_trie;
  vector<uint32_t> ids;
  label_trie.convert(labels, ids);

  cout << "transforming main trie" << endl;
  LoudsMap map;
  map.build(trie0);
  map.updateLinks(ids);
  map.set_label_trie(&label_trie);
  cout << "done" << endl;

  TrieValueType value;
  //map.display();
  int count = tokens.size();
  for (int i = 0; i < count; ++i) {
    const char *key = tokens[i].c_str();
    bool found = map.findEntry(key, value);
    EXPECT_TRUE(found);
    EXPECT_EQ(value, i+ 1);
  }

  for (int i = 0; i < count; ++i) {
    const char *key = tokens[i].c_str();
    bool found = map.findEntryLowerBound(key, NULL, value);
    EXPECT_TRUE(found);
    EXPECT_EQ(value, i+ 1);
  }
}

