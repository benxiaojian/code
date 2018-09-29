#include <iostream>
#include "hello.h"
#include <gtest/gtest.h>

TEST(AddTet, add_test)
{
    Add a;
	int res = a.add(1,2);

	EXPECT_EQ(3, res);
}

TEST(AddTet, fail_to_add_test)
{
    Add a;
	int res = a.add(1,2);

	EXPECT_EQ(4, res);
}

int main(int argc, char *argv[])
{
   ::testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
}
