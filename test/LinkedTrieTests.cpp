#include <gtest/gtest.h>
#include "LinkedTrie.h"

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

TEST_F(LinkedTrieTests, Dfuds) {
	_node->addEntry("zzd", 3);
	_node->addEntry("gook", 4);
	_node->addEntry("test", 44);

	_node->buildDfudsTrie();
	cout << endl;
	_node->write(cout);
	cout << "|" << endl << endl;
	
	uint64_t value = _node->getEntry2("test");
	EXPECT_EQ(44, value);
}

TEST_F(LinkedTrieTests, size) {

	uint32_t size = _node->sizeInByte();
	uint32_t expected = 56;
	EXPECT_EQ(expected, size);

	const char *key = "abc";
	uint64_t value = 33;
	_node->addEntry(key, value);

	size = _node->sizeInByte();
	EXPECT_LT(expected, size);

}


