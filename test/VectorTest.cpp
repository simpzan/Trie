#include <gtest/gtest.h>

#include <sstream>
#include "Vector.h"

TEST(VectorTest, append) {
	Vector<char> v;
	v.append('a');
    EXPECT_EQ('a', v[0]);

	uint64_t count = v.count();
	EXPECT_EQ(1, count);

	uint64_t size = v.size();
	EXPECT_GE(size, 0);

	v.clear();
	count = v.count();
	EXPECT_EQ(0, count);
}

TEST(VectorTest, readWrite) {
	Vector<char> v;
	v.append('a');
	v.append('Z');
	
	std::stringstream ss;
	v.write(ss);

	Vector<char> v2;
	v2.read(ss);
	uint64_t count = v2.count();
	EXPECT_EQ(2, count);
	EXPECT_EQ('Z', v2[1]);
}


