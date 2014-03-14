#include <gtest/gtest.h>
#include "PTrieLoudsBuilder.h"
#include "PTrieLouds.h"
#include "utils.h"

using namespace std;

TEST(PTrieLoudsBuilderTest, test) {
	std::vector<std::string> v;
	v.push_back("thank");
	v.push_back("that");
    v.push_back("then");
	PTrieLoudsBuilder builder(v);
	builder.build("tmp");

	PTrieLouds trie;
	trie.load("tmp");
    
	uint32_t value;
	bool found = trie.find("then", value);
    EXPECT_EQ(true, found);
    EXPECT_EQ(2, value);
    
    value = 0;
    found = trie.find("kkk", value);
    EXPECT_EQ(false, found);
    EXPECT_EQ(0, value);
}

void checkStrings(vector<string> &tokens) {
    cout << "token count:" << tokens.size() << endl;
    if (tokens.size() ==0) return;
    
    string lastkey = tokens[0];
    assert(lastkey.length() > 0);
    for (int i=1; i<tokens.size(); ++i) {
        string thiskey = tokens[i];
        assert(thiskey > lastkey);
        lastkey = thiskey;
    }
//    tokens.resize(tokens.size() - 1);
}

//TEST(PTrieLoudsBuilderTest, run) {
//    PTrieLouds trie;
//    trie.load("testdata/words.ptrielouds");
//   	uint32_t value;
//	bool found = trie.find("then", value);
//    EXPECT_EQ(true, found);
//    EXPECT_EQ(2, value);
//    
//    value = 0;
//    found = trie.find("kkk", value);
//    EXPECT_EQ(false, found);
//    EXPECT_EQ(0, value);
//}

TEST(PTrieLoudsBuilderTest, build) {
    string filename("testdata/words");
    vector<string> tokens;
    loadTokensFromFile(filename.c_str(), tokens);
    checkStrings(tokens);
    PTrieLoudsBuilder builder(tokens);
    builder.build("words.ptrielouds.tmp");
}
