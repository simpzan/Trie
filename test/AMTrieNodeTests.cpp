#include "AMTrieNode.h"
#include <gtest/gtest.h>
#include <cstdlib>
#include <algorithm>

using namespace std;

class AMTrieNodeTests: public ::testing::Test {
	protected:

	TrieNode *_node;

	virtual void SetUp() {
		_node = new AMTrieNode;
	}

	virtual void TearDown() {
		delete _node;
	}
};

TEST_F(AMTrieNodeTests, setGetChildInThisNode) {

	TrieNode *child = _node->getChildNodeWithLabel('c');
	ASSERT_TRUE(child == NULL);

	TrieNode *node = new AMTrieNode;
	_node->setChildNodeWithLabel('k', node);
	
	TrieNode *foundNode = _node->getChildNodeWithLabel('k');
	ASSERT_EQ(node, foundNode);

	foundNode = _node->getChildNodeWithLabel('j');
	ASSERT_EQ(NULL, foundNode);

	foundNode = _node->getChildNodeWithLabel('l');
	ASSERT_EQ(NULL, foundNode);
}

TEST_F(AMTrieNodeTests, setGetValueInThisNode) {
	uint64_t value = _node->getValue();
	ASSERT_EQ(0, value);

	uint64_t valueToInsert = 44;
	_node->setValue(valueToInsert);
	uint64_t valueRetrieved = _node->getValue();
	ASSERT_EQ(valueToInsert, valueRetrieved);
}

int stringCompare(const void *arg1, const void *arg2);

TEST_F(AMTrieNodeTests, prefixRange) {

	// test empty content
	const char *prefix = "测试";
	vector<Entry> entries;
	bool result = _node->prefix_range(prefix, entries);
	bool expected = false;
	ASSERT_EQ(expected, result);
	ASSERT_EQ(0, entries.size());

	// insert 
	string strings[] = {"g"};
	//string strings[] = {"ab哈的c", "xy他的z", "mnp", "efg"};
	int count = sizeof(strings)/sizeof(strings[0]);
	for (int i=0; i<count; ++i) {
		string word(prefix + strings[i]);
		strings[i] = word;
		_node->addEntry(word.c_str(), word.size());
	}

	// test insert succeeded.
	result = _node->prefix_range("", entries);
	ASSERT_EQ(true, result);
	ASSERT_EQ(count, entries.size());

	qsort(strings, count, sizeof(strings[0]), stringCompare);
	for (int i=0; i<count; ++i) {
		const char *expected = strings[i].c_str();
		const char *actual = entries[i].first.c_str();
		EXPECT_STREQ(expected, actual);
	}
}


TEST_F(AMTrieNodeTests, addGetValue) {

	//const char *strings[] = {"abc", "xyz", "mnp", "efg"};
	const char *strings[] = {"\255\254", "\253"};
	int count = sizeof(strings)/sizeof(*strings);
	for (int i=0; i<count; ++i) {
		_node->addEntry(strings[i], strlen(strings[i]));
	}

	for (int i=0; i<count; ++i) {
		const char* str = strings[i];
		uint64_t value = _node->getEntry(str);
		EXPECT_EQ(strlen(str), value);
	}
}

TEST_F(AMTrieNodeTests, size) {
	uint32_t size = _node->sizeInByte();
	cout << "ArrayTrieNode size(bytes): " << size << endl;
	uint32_t expected = _node->sizeOfThisNode();
	EXPECT_EQ(expected, size);

	uint32_t sizeInMB = _node->sizeInMegaByte();
	expected /= (1024*1024);
	EXPECT_EQ(expected, sizeInMB);

	_node->addEntry("abc", 3);
	size = _node->sizeInByte();
	EXPECT_LT(expected, size);

	sizeInMB = _node->sizeInMegaByte();
	expected /= (1024*1024);
	EXPECT_EQ(expected, sizeInMB);
}

TEST_F(AMTrieNodeTests, benchmark) {
	
}
