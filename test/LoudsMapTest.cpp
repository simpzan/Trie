#include <gtest/gtest.h>
#include "LoudsMap.h"
#include "LoudsTrie.h"
#include "PTrie.h"
#include "Trie.h"
#include "utils.h"
#include "LoudsMapBuilder.h"
#include "LoudsTrieBuilder.h"
#include "timer.h"

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
  
  typedef LoudsTrie<sdsl::bit_vector> LoudsTrie;
  typedef LoudsMap<sdsl::bit_vector, Vector<uint32_t>, LoudsTrie> LoudsMap;

  vector<string> tokens;
  //tokens.push_back("tester");
  //tokens.push_back("testing");
  //tokens.push_back("xyz");
  //tokens.push_back("xyzing");
  loadTokensFromFile("block", tokens);

  cout << "build main trie" << endl;
  PTrie trie0;
  addEntries(tokens, trie0);

  LinkedTrie labels;
  trie0.collectLabels(labels);

  cout << "building label trie" << endl;
  LoudsTrieBuilder builder;
  vector<uint32_t> ids;
  labels.convert(builder, ids);

  LoudsTrie label_trie;
  label_trie.init(builder);

  cout << "transforming main trie" << endl;
  LoudsMapBuilder builder1;
  builder1.build(trie0);
  builder1.updateLinks(ids);
  //builder1.display();

  LoudsMap map;
  map.init(builder1);
  map.set_label_trie(&label_trie);
  cout << "done" << endl;

  TrieValueType value;
  //map.display();
  int count = tokens.size();
  for (int i = 0; i < count; ++i) {
    const char *key = tokens[i].c_str();
    bool found = map.findEntry(key, value);
    assert(found);
    ASSERT_TRUE(found);
    EXPECT_EQ(value, i+ 1);
  }

  for (int i = 0; i < count; ++i) {
    const char *key = tokens[i].c_str();
    bool found = map.findEntryLowerBound(key, NULL, value);
    EXPECT_TRUE(found);
    EXPECT_EQ(value, i+ 1);
  }
}

