#include <fstream>
#include <gtest/gtest.h>

#include "DfudsMapBuilder.h"
#include "DfudsMap.h"

using namespace std;

TEST(DfudsMapTest, t) {
	ifstream is("words.tiny");
	assert(is.good());
	string word;

	DfudsMapBuilder<uint64_t> builder;
	builder.set_is_leaf(false);
	int i = 0;
	while (is.good()) {
		is >> word;
		//cout << word << endl;
		++i;
		builder.addEntry(word.c_str(), i);
	}

	fstream ss("t.a", ios::in|ios::out|ios::trunc);
	builder.write(ss);
	ss.seekg(0);

	DfudsMap<uint64_t> map;
	map.load(ss);

	uint64_t value = 0;
	bool result = map.find("taction", value);
	EXPECT_TRUE(result);
	EXPECT_EQ(8, value);
}

TEST(DfudsMapTest, test) {
	DfudsMapBuilder<uint64_t> builder;	
	builder.addEntry("zzd", 3);
	builder.addEntry("gook", 4);
	builder.addEntry("test", 44);

	stringstream ss;
	builder.write(ss);

	DfudsMap<uint64_t> map;
	map.load(ss);

	uint64_t value = 0;
	bool result = map.find("test", value);
	EXPECT_TRUE(result);
	EXPECT_EQ(44, value);
}
