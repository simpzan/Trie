#include <gtest/gtest.h>
#include <cstdlib>
#include <algorithm>
#include "ArrayTrieNode.h"

using namespace std;

class ArrayTrieNodeTests: public ::testing::Test {
protected:
	ArrayTrieNode _node;

	virtual void SetUp() { 

	}

	virtual void TearDown() {
	
	}

};


TEST_F(ArrayTrieNodeTests, setGetChildInThisNode) {
	ArrayTrieNode *child = _node.getChildNodeWithLabel('c');
	ASSERT_TRUE(child == NULL);

	ArrayTrieNode *node = new ArrayTrieNode;
	_node.setChildNodeWithLabel('k', node);
	ArrayTrieNode *foundNode = _node.getChildNodeWithLabel('k');
	ASSERT_EQ(node, foundNode);

	foundNode = _node.getChildNodeWithLabel('c');
	ASSERT_EQ(NULL, foundNode);
}

TEST_F(ArrayTrieNodeTests, setGetValueInThisNode) {
	uint64_t value = _node.getValueWithLabel('c');
	ASSERT_EQ(0, value);

	uint64_t valueToInsert = 44;
	_node.setValueWithLabel('c', valueToInsert);
	uint64_t valueRetrieved = _node.getValueWithLabel('c');
	ASSERT_EQ(valueToInsert, valueRetrieved);
}

int stringCompare(const void *arg1, const void *arg2) {
	const char *str1 = * (const char **) arg1;
	const char *str2 = * (const char **) arg2;
	int returnValue = strcmp(str1, str2);
	return returnValue;
}

TEST_F(ArrayTrieNodeTests, prefixRange) {
	const char *prefix = "测试";
	vector<Entry> entries;
	bool result = _node.prefix_range(prefix, entries);
	ASSERT_EQ(false, result);
	ASSERT_EQ(0, entries.size());

	string strings[] = {"ab哈的c", "xy他的z", "mnp", "efg"};
	int count = sizeof(strings)/sizeof(strings[0]);
	for (int i=0; i<count; ++i) {
		string word(prefix + strings[i]);
		strings[i] = word;
		_node.addValue(word.c_str(), word.size());
	}

	result = _node.prefix_range(prefix, entries);
	ASSERT_EQ(true, result);
	ASSERT_EQ(count, entries.size());

	qsort(strings, count, sizeof(strings[0]), stringCompare);
	for (int i=0; i<count; ++i) {
		const char *expected = strings[i].c_str();
		const char *actual = entries[i].first.c_str();
		EXPECT_STREQ(expected, actual);
	}
}


TEST_F(ArrayTrieNodeTests, addGetValue) {
	const char *strings[] = {"abc", "xyz", "mnp", "efg"};
	int count = sizeof(strings)/sizeof(*strings);
	for (int i=0; i<count; ++i) {
		_node.addValue(strings[i], strlen(strings[i]));
	}

	for (int i=0; i<count; ++i) {
		const char* str = strings[i];
		uint64_t value = _node.getValue(str);
		EXPECT_EQ(strlen(str), value);
	}
}

TEST_F(ArrayTrieNodeTests, size) {
	uint32_t size = _node.sizeInByte();
	uint32_t expected = NODESIZE*(8+8);
	EXPECT_EQ(expected, size);

	uint32_t sizeInMB = _node.sizeInMegaByte();
	expected /= (1024*1024);
	EXPECT_EQ(expected, sizeInMB);

	_node.addValue("abc", 3);
	size = _node.sizeInByte();
	expected = 3*NODESIZE*(8+8);
	EXPECT_EQ(expected, size);

	sizeInMB = _node.sizeInMegaByte();
	expected /= (1024*1024);
	EXPECT_EQ(expected, sizeInMB);
}
