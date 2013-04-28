#include <gtest/gtest.h>
#include <sstream>

#include "BitVectorBuilder.h"
#include "ConstBitVector.h"

using namespace std;

TEST(BitVectorTest, test) {
	BitVectorBuilder bvb;

	bvb.append(false);
	bvb.append(true);
	bvb.append(true);
	bvb.append(true);

	bvb.append(false);
	bvb.append(true);
	bvb.append(false);
	bvb.append(true);
	bvb.append(true);

	stringstream ss;
	bvb.write(ss);


	ConstBitVector cbv;
	cbv.read(ss);
	EXPECT_EQ(false, cbv[0]);
	EXPECT_EQ(true, cbv[1]);
	EXPECT_EQ(true, cbv[2]);
	EXPECT_EQ(true, cbv[3]);
	EXPECT_EQ(false, cbv[6]);
	EXPECT_EQ(true, cbv[7]);
	EXPECT_EQ(true, cbv[8]);
	
	uint64_t offset = 3;
	uint64_t count = cbv.rank1(offset);
	uint64_t expected = 3;
	EXPECT_EQ(expected, count);

	offset = cbv.select1(count);
	expected = 3;
	EXPECT_EQ(expected, offset);

	offset = 6;
	count = cbv.rank0(offset);
	expected = 3;
	EXPECT_EQ(expected, count);

	offset = cbv.select0(count);
	expected = 6;
	EXPECT_EQ(expected, offset);

	offset = 4;
	uint64_t closeOffset = cbv.findClose(offset);
	expected = 5;
	EXPECT_EQ(expected, closeOffset);

	offset = 6;
	closeOffset = cbv.findClose(offset);
	expected = 7;
	EXPECT_EQ(expected, closeOffset);
}

TEST(BitVectorTest, findOpen) {
	BitVectorBuilder bvb;

	bvb.append(false);

	bvb.append(false);
	bvb.append(true);
	
	bvb.append(false);
	bvb.append(true);
	
	bvb.append(false);
	bvb.append(false);
	bvb.append(true);

	bvb.append(true);

	bvb.append(true);

	stringstream ss;
	bvb.write(ss);

	ConstBitVector cbv;
	cbv.read(ss);

	uint64_t offset = 9;
	uint64_t expected = 0;
	uint64_t openOffset = cbv.findOpen(offset);
	EXPECT_EQ(expected, openOffset);

	offset = 7;
	expected = 6;
	openOffset = cbv.findOpen(offset);
	EXPECT_EQ(expected, openOffset);
}
