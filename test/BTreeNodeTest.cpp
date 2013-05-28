#include <gtest/gtest.h>
#include "BTreeNode.h"
#include "BTreeNodeBuilder.h"

using namespace std;

typedef uint32_t ValueT;

TEST(BTreeNodeTest, test) {
  BTreeNodeBuilder<ValueT> builder;
  builder.set_block_size(1024*4);
  builder.set_is_leaf(false);

  const char *key = "key";
  ValueT expected = 3;
  builder.addEntry(key, expected);

  stringstream ss;
  builder.save(ss);

  BTreeNode<ValueT> node;
  node.load(ss);

  ValueT value;
  node.find(key, value);
  EXPECT_EQ(expected, value);

  key = "test";
  expected = 0;
  node.find(key, value);
  EXPECT_EQ(expected, value);

  key = "kay";
  expected = 3;
  node.lowerBound(key, value);
  EXPECT_EQ(expected, value);
}

