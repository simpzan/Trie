#include <gtest/gtest.h>
#include "LinkedTrie.h"
#include "DfudsTrie.h"

using namespace std;

class LinkedTrieTests: public ::testing::Test {
protected:
	LinkedTrie *_node;
	
	void SetUp() {
		_node = new LinkedTrie();
	}

	void TearDown() {
		delete _node;
	}
};

TEST_F(LinkedTrieTests, setGetEntry) {
	const char *key = "abc";
	uint64_t value = _node->getEntry(key);
	EXPECT_EQ(0, value);

	uint64_t expected = 34;
	_node->addEntry(key, expected);
	value = _node->getEntry(key);
	EXPECT_EQ(expected, value);
}



