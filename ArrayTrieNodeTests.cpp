#include <gtest/gtest.h>
#include "ArrayTrieNode.h"

class ArrayTrieNodeTests: public ::testing::Test {
protected:
	ArrayTrieNode _node;

	virtual void SetUp() { 

	}

	virtual void TearDown() {
	
	}

};


TEST_F(ArrayTrieNodeTests, findChild) {
	ArrayTrieNode *child = _node.findChildNodeWithLabel('c');
	ASSERT_TRUE(child == NULL);

	ArrayTrieNode *node = new ArrayTrieNode;
	_node.setChildNodeWithLabel('k', node);
	ArrayTrieNode *foundNode = _node.findChildNodeWithLabel('k');
	ASSERT_EQ(node, foundNode);

	foundNode = _node.findChildNodeWithLabel('c');
	ASSERT_EQ(NULL, foundNode);

	delete node;
}

TEST_F(ArrayTrieNodeTests, setGetValue) {
	uint64_t value = _node.getValueWithLabel('c');
	ASSERT_EQ(0, value);

	uint64_t valueToInsert = 44;
	_node.setValueWithLabel('c', valueToInsert);
	uint64_t valueRetrieved = _node.getValueWithLabel('c');
	ASSERT_EQ(valueToInsert, valueRetrieved);

}

TEST_F(ArrayTrieNodeTests, getStringsInSubTrie) {
	

}

TEST_F(ArrayTrieNodeTests, addNodes) {
	const char *strings[] = {"abc", "xyz", "mnp", "efg"};
	int count = sizeof(strings)/sizeof(*strings);
	for (int i=0; i<count; ++i) {
		string str(strings[i]);
		_node.addNodesWithKey(str, 0);
	}

	for (int i=0; i<count; ++i) {
		string str(strings[i]);
		int prefixLen = 0;
		_node.searchNodeWithKey(str, prefixLen);

		ASSERT_EQ(str.size()-1, prefixLen);	
	}
}
