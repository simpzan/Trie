#include <gtest/gtest.h>
#include <sstream>

#include "DfudsTrie.h"
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
