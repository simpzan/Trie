#include <gtest/gtest.h>
#include "BitVectorBuilder.h"

using namespace std;

TEST(BitVectorBuilderTest, test) {
	BitVectorBuilder builder;
	for (int i=0; i<32; ++i) {
		builder.addBit(i % 2);
	}
}