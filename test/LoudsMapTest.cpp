#include <gtest/gtest.h>
#include "LoudsMap.h"
#include "LoudsTrie.h"
#include "PTrie.h"
#include "Trie.h"
#include "utils.h"

using namespace std;

TEST(LoudsMapTest, computePrefix) {
  const char *tokens[] = {"est", "r", "ng", "yz"};
  vector<LinkedTrieNode *> nodes;
  LinkedTrie trie0;
  int count = sizeof(tokens) / sizeof(tokens[0]);
  for (int i = 0; i < count; ++i) {
    const char *token = tokens[i];
    string reversed;
    reverseKey(token, reversed);
    LinkedTrieNode *node = (LinkedTrieNode *) trie0.addKey(reversed.c_str());
    nodes.push_back(node);
  }

  LoudsTrie trie;
  trie.build(trie0);

  count = nodes.size();
  for (int i = 0; i < count; ++i) {
    LinkedTrieNode *node = nodes[i];
    string prefix;
    trie.computePrefix(node->get_id(), prefix);
    cout << prefix << endl;
  }

}

TEST(LoudsMapTest, t1) {
  vector<string> tokens;
  loadTokensFromFile("block", tokens);

  cout << "build main trie" << endl;
  PTrie trie0;
  int count = tokens.size();
  for (int i = 0; i < count; ++i) {
    const char *token = tokens[i].c_str();
    trie0.addEntry(token, i + 1);
  }

  cout << "collecting labels" << endl;
  vector<string> labels;
  trie0.collectLabels(labels);

  cout << "building label trie" << endl;
  vector<TrieNode *> tail_nodes;
  LinkedTrie label_trie0;
  label_trie0.addKeys(labels, tail_nodes);
  
  cout << "transforming label trie" << endl;
  LoudsTrie label_trie;
  label_trie.build(label_trie0);

  cout << "collecting link info" << endl;
  vector<uint32_t> ids;
  label_trie0.generateIds(tail_nodes, ids);

  cout << "transforming main trie" << endl;
  LoudsMap map;
  map.build(trie0);
  map.updateLinks(ids);
  map.set_label_trie(&label_trie);
  cout << "done" << endl;

  TrieValueType value;
  //map.display();
  count = tokens.size();
  for (int i = 0; i < count; ++i) {
    const char *key = tokens[i].c_str();
    bool found = map.findEntry(key, value);
    EXPECT_TRUE(found);
    EXPECT_EQ(value, i+ 1);
  }
}

TEST(LoudsMapTest, test) {
  return;
  vector<string> tokens;
  loadTokensFromFile("top.0", tokens);

  cout << "build main trie" << endl;
  PTrie trie0;
  int count = tokens.size();
  for (int i = 0; i < count; ++i) {
    const char *token = tokens[i].c_str();
    trie0.addEntry(token, i + 1);
  }

  cout << "collecting labels" << endl;
  vector<string> labels;
  trie0.collectLabels(labels);

  cout << "building label trie" << endl;
  vector<LinkedTrieNode *> tail_nodes;
  LinkedTrie label_trie0;
  count = labels.size();
  for (int i = 0; i < count; ++i) {
    string label = labels[i];
    //cout << label << endl;
    string reversed;
    reverseKey(label.c_str(), reversed);
    LinkedTrieNode *node = (LinkedTrieNode *)label_trie0.addKey(reversed.c_str());
    tail_nodes.push_back(node);
  }

  cout << "transforming label trie" << endl;
  LoudsTrie label_trie;
  label_trie.build(label_trie0);

  cout << "updating link info" << endl;
  vector<uint32_t> ids;
  count = tail_nodes.size();
  for (int i = 0; i < count; ++i) {
    LinkedTrieNode *node = tail_nodes[i];
    ids.push_back(node->get_id());
  }

  //label_trie.display();
  //for (int i = 0; i < count; ++i) {
    //string label = labels[i];
    //uint32_t id = ids[i];
    //cout << labels[i] << " - " << ids[i] << endl;
    //string prefix;
    //label_trie.computePrefix(id, prefix);
    //assert(label == prefix);
  //}


  cout << "transforming main trie" << endl;
  LoudsMap map;
  map.build(trie0);
  map.updateLinks(ids);

  map.set_label_trie(&label_trie);

  cout << "done" << endl;
  const char *key = "divaricatingly";
  TrieValueType value;
  map.display();
  count = tokens.size();
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

  vector<string> keys;
  loadTokensFromFile("words.sorted", keys);
  count = keys.size();
  string last_found;
  for (int i = 0; i < count; ++i) {
    string key = keys[i];
    string key_found;
    bool found = map.findEntryLowerBound(key.c_str(), &key_found, value);
    EXPECT_TRUE(found);
    ASSERT_TRUE(key_found >= key) << key_found << " " << key << endl;
    
    //if (last_found != key_found) {
      //cout << key_found << " " << i << endl;
    //}

    last_found = key_found;
  }

}

