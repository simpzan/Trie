#include <vector>
#include <string>
#include <gtest/gtest.h>
#include "FrontCoder.h"

using namespace std;

TEST(FrontCoderTest, test) {
  vector<string> keys;
  keys.push_back("abc");
  keys.push_back("abcd");
  keys.push_back("abce");
  keys.push_back("zzzz");
  
  FrontCoderBuilder coderBuilder;
  for (int i=0; i<keys.size(); ++i) {
    coderBuilder.add(keys[i]);
  }

  stringstream ss;
  coderBuilder.save(ss);

  FrontCoder coder;
  coder.load(ss);

  string key("abce");
  uint32_t rank;
  bool found = coder.find(key, rank);
  EXPECT_TRUE(found);
  EXPECT_EQ(2, rank);

  key = "a";
  found = coder.find(key, rank);
  EXPECT_FALSE(found);
  EXPECT_EQ(0, rank);

  key = "z";
  found = coder.find(key, rank);
  EXPECT_FALSE(found);
  EXPECT_EQ(3, rank);

  key = "zzzz";
  found = coder.find(key, rank);
  EXPECT_TRUE(found);
  EXPECT_EQ(3, rank);

  key = "zzzzzzzz";
  found = coder.find(key, rank);
  EXPECT_FALSE(found);
  EXPECT_EQ(4, rank);

  key = "A";
  found = coder.find(key, rank);
  EXPECT_FALSE(found);
  EXPECT_EQ(0, rank);
}

