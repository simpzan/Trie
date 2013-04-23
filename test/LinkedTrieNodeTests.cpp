#include <gtest/gtest.h>
#include "LinkedTrieNode.h"

using namespace std;

class LinkedTrieNodeTests: public ::testing::Test {
	protected:
		LinkedTrieNode *_node;

		void SetUp() {
			_node = new LinkedTrieNode();
		}

		void TearDown() {
			delete _node;
		}

};

TEST_F(LinkedTrieNodeTests, setGetValueInThisNode) {
	uint64_t value = _node->getValue();
	ASSERT_EQ(0, value);

	uint64_t valueToInsert = 44;
	_node->setValue(valueToInsert);
	uint64_t valueRetrieved = _node->getValue();
	ASSERT_EQ(valueToInsert, valueRetrieved);
}
