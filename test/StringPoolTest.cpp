#include <gtest/gtest.h>
#include "StringPool.h"


using namespace std;

TEST(StringPoolTest, test) {
  StringPool pool;
  uint32_t id = pool.findOrInsert("ping");
  EXPECT_EQ(0, id);
  id = pool.findOrInsert("ping");
  EXPECT_EQ(0, id);
  id = pool.findOrInsert("king");
  EXPECT_EQ(1, id);
  pool.build();
//  pool.serialize(cout);
  Vector<uint32_t> ids(pool.newIds());
  ids.display(cout);
}
