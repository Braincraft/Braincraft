#include "gtest/gtest.h"
//g++ test_gtest.cpp -o canard -lgtest
TEST(FooTest, MethodBarDoesAbc) {
ASSERT_TRUE(1 != NULL);
  EXPECT_EQ(2, 1);
}
TEST(FooTest, DoesXyz) {
}
int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
