#include <fstream>
#include <gtest/gtest.h>

#include "DfudsMapBuilder.h"
#include "DfudsMap.h"

typedef uint32_t ValueT;
using namespace std;

TEST(DfudsMapTest, t) {
  return;
	ifstream is("words.tiny");
	assert(is.good());
	string word;

	DfudsMapBuilder<ValueT> builder;
	builder.set_is_leaf(false);
	int i = 0;
	while (is.good()) {
		is >> word;
		//cout << word << endl;
		++i;
		builder.addEntry(word.c_str(), i);
	}

  stringstream ss;
	builder.write(ss);
	ss.seekg(0);

	DfudsMap<ValueT> map;
	map.load(ss);

	ValueT value = 0;
	bool result = map.find("taction", value);
	EXPECT_TRUE(result);
	EXPECT_EQ(8, value);
}

TEST(DfudsMapTest, test) {
	DfudsMapBuilder<ValueT> builder;	
	builder.addEntry("zzd", 3);
	builder.addEntry("gook", 4);
	builder.addEntry("test", 44);

	stringstream ss;
	builder.write(ss);

	DfudsMap<ValueT> map;
	map.load(ss);

	ValueT value = 0;
	bool result = map.find("test", value);
	EXPECT_TRUE(result);
	EXPECT_EQ(44, value);
}
