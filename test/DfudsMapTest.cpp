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
	builder.save(ss);
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
	builder.save(ss);

	DfudsMap<ValueT> map;
	map.load(ss);

	ValueT value = 0;
	bool result = map.find("test", value);
	EXPECT_TRUE(result);
	EXPECT_EQ(44, value);
}

void findLCPTest(DfudsMapLCP<ValueT> &map, const char *key, ValueT e, int count) {
  for (int i = 0; i <= count; ++i) {
    uint8_t lcp = i;
    ValueT value = 0;
    bool found = map.findWithLCP(key, value, &lcp);
    EXPECT_TRUE(found);
    EXPECT_EQ(e, value);
  }
}

void lowerBoundLCPTest(DfudsMapLCP<ValueT> &map, const char *key, ValueT e,
    int count) {
  for (int i = 0; i < count; ++i) {
    uint8_t lcp = i;
    ValueT value = 0;
    bool found = map.lowerBoundWithLCP(key, value, &lcp);
    EXPECT_EQ(e, value);
  }
}

TEST(DfudsMapLCPTest, test) {
	DfudsMapLCPBuilder<ValueT> builder;	
	builder.addEntry("zzd", 3);
	builder.addEntry("gook", 4);
	builder.addEntry("test", 44);
	builder.addEntry("tesy", 45);

	stringstream ss;
	builder.save(ss);

	DfudsMapLCP<ValueT> map;
	map.load(ss);

  findLCPTest(map, "test", 44, 3);
  findLCPTest(map, "tesy", 45, 4);

  lowerBoundLCPTest(map, "tesy", 45, 4);
  lowerBoundLCPTest(map, "tesx", 45, 3);
}
