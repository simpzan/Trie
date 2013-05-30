#include <gtest/gtest.h>

#include <sstream>
#include "utils.h"
#include "DfudsTrie.h"
#include <fstream>
#include "DfudsTrieBuilder.h"

using namespace std;

TEST(DfudsTrieTest, rightNearFind) {
	DfudsTrieBuilder builder;
	//builder.addEntry("go", 232);
	builder.addEntry("good", 4);
	builder.addEntry("test", 44);
	builder.addEntry("yyd", 3);

	stringstream ss;
	builder.write(ss);
	
	DfudsTrie trie;
	trie.read(ss);

	uint64_t rank = trie.rightNearFind("a");
	EXPECT_EQ(1, rank);

	rank = trie.rightNearFind("food");
	EXPECT_EQ(1, rank);

	rank = trie.rightNearFind("good");
	EXPECT_EQ(1, rank);

	rank = trie.rightNearFind("goodtest");
	EXPECT_EQ(2, rank);

	rank = trie.rightNearFind("hood");
	EXPECT_EQ(2, rank);

	rank = trie.rightNearFind("tess");
	EXPECT_EQ(2, rank);

	rank = trie.rightNearFind("test");
	EXPECT_EQ(2, rank);

	rank = trie.rightNearFind("teyy");
	EXPECT_EQ(3, rank);

	rank = trie.rightNearFind("yyc");
	EXPECT_EQ(3, rank);

	rank = trie.rightNearFind("yyd");
	EXPECT_EQ(3, rank);

	rank = trie.rightNearFind("z");
	EXPECT_EQ(0, rank);
}

TEST(DfudsTrieTest, right) {
	DfudsTrieBuilder builder;
	//builder.addEntry("go", 232);
	builder.addEntry("offish", 4);
	builder.addEntry("oleomargarine", 44);
	builder.addEntry("om", 3);

	stringstream ss;
	builder.write(ss);
	
	DfudsTrie trie;
	trie.read(ss);

	uint64_t rank = trie.rightNearFind("oleometer");
	EXPECT_EQ(3, rank);
}

TEST(DfudsTrieTest, find) {
	DfudsTrieBuilder builder;
	builder.addEntry("g", 1);
	builder.addEntry("good", 4);
	builder.addEntry("test", 44);
	builder.addEntry("yyd", 3);
	builder.addEntry("yy", 1);

	stringstream ss;
	builder.write(ss);
	
	DfudsTrie trie;
	trie.read(ss);

	uint64_t rank = trie.find("test");
	EXPECT_EQ(3, rank);

	rank = trie.find("yyd");
	EXPECT_EQ(5, rank);

	rank = trie.find("good");
	EXPECT_EQ(2, rank);

	rank = trie.find("g");
	EXPECT_EQ(1, rank);

	rank = trie.find("yy");
	EXPECT_EQ(4, rank);
}

TEST(DfudsTrieTest, select) {
	DfudsTrieBuilder builder;
	builder.addEntry("g", 1);
	builder.addEntry("good", 4);
	builder.addEntry("test", 44);
	builder.addEntry("yyd", 3);
	builder.addEntry("yy", 1);

	stringstream ss;
	builder.write(ss);
	
	DfudsTrie trie;
	trie.read(ss);

	string key;
	trie.select(1, key);
	EXPECT_STREQ("g", key.c_str());

	trie.select(3, key);
	EXPECT_STREQ("test", key.c_str());
}

TEST(DfudsTrieTest, kk) {
	ifstream is("oleometer.words");
	assert(is.good());
	string word;

	DfudsTrieBuilder builder;
	int i = 0;
	while (is.good()) {
		is >> word;
		//cout << word << endl;
		++i;
		builder.addEntry(word.c_str(), i);
	}

	stringstream ss;
	builder.write(ss);

	DfudsTrie map;
	map.read(ss);

  uint64_t value = map.rightNearFind("oleometer");
  EXPECT_EQ(7, value);
	//uint64_t id = map._dfuds.findClose(13);
	//EXPECT_EQ(96, id);
}

TEST(DfudsTrieTest, findClose) {
	ifstream is("findClose.words");
	assert(is.good());
	string word;

	DfudsTrieBuilder builder;
	int i = 0;
	while (is.good()) {
		is >> word;
		++i;
		builder.addEntry(word.c_str(), i);
	}

	stringstream ss;
	builder.write(ss);

	DfudsTrie map;
	map.read(ss);

	uint64_t value = map.rightNearFind("Awabakal");
	//EXPECT_EQ(7, value);
}

void findLCPTest(const DfudsTrieLCP &trie, const char *key, const uint64_t e, 
    int count) {
  for (int i = 0; i <= count; ++i) {
    uint8_t lcp = i;
    uint64_t rank = trie.findLCP(key, &lcp);
    EXPECT_EQ(e, rank);
  }
}

void rightNearFindLCPTest(const DfudsTrieLCP &trie, const char *key, 
    const uint64_t e, int count) {
  for (int i = 0; i <= count; ++i) {
    uint8_t lcp = i;
    uint64_t rank = trie.rightNearFindLCP(key, &lcp);
    EXPECT_EQ(e, rank);
    string key_found;
    trie.select(rank, key_found);
    int lcp_e = computeLCP(key, key_found.c_str());
    EXPECT_EQ(lcp_e, lcp);
  }
}

TEST(DfudsTrieLCPTest, test) {
	DfudsTrieLCPBuilder builder;
	builder.addEntry("g", 1);
	builder.addEntry("good", 4);
	builder.addEntry("test", 44);
	builder.addEntry("yy", 1);
	builder.addEntry("yydddd", 3);
	builder.addEntry("yyg", 3);

	stringstream ss;
	builder.write(ss);
	
	DfudsTrieLCP trie;
	trie.read(ss);

  findLCPTest(trie, "yyg", 6, 3);
  findLCPTest(trie, "yydddd", 5, 2);
  findLCPTest(trie, "yy", 4, 2);

  rightNearFindLCPTest(trie, "yyg", 6, 3);
  rightNearFindLCPTest(trie, "yyddk", 6, 2);
}
