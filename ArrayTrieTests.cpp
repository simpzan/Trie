#include <gtest/gtest.h>
#include "ArrayTrie.h"


using namespace std;


class ArrayTrieTests: public ::testing::Test {
	protected:
		ArrayTrie at;

		virtual void SetUp() {

		}

		virtual void TearDown() {

		}


};

TEST_F(ArrayTrieTests, addEntry) {
	uint64_t value = at.getEntry("tsd");
	EXPECT_EQ(0, value);

	at.addEntry("abcde", 5);
	at.addEntry("zbdtyg", 6);
	at.addEntry("tsd", 3);

	 value = at.getEntry("tsd");
	EXPECT_EQ(3, value);
}

TEST_F(ArrayTrieTests, prefixRange) {

	std::vector<std::string> stringV;
	bool result = at.prefix_range("abc", stringV);
	ASSERT_EQ(false, result);
	ASSERT_EQ(0, stringV.size());

	const char *str[] = { "abcdef", "abcabc", "abcxyz", "abcmnp", "xyz"};
	int len = sizeof(str)/sizeof(*str);
	for (int i=0; i<len; ++i) {
		at.addEntry(str[i], strlen(str[i]));
	}
	
	result = at.prefix_range("abc", stringV);
	ASSERT_EQ(true, result);
	ASSERT_EQ(4, stringV.size());
	
	const char *expectedStrings[] = {"abcabc", "abcdef", "abcmnp", "abcxzy"};
	for (int i=0; i<4; ++i) {
		const char* word = stringV[i].c_str();
		const char* expected = expectedStrings[i];
		EXPECT_STREQ(expected, word);
	}
}
