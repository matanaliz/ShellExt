// unittest_CppShellExt.cpp : Defines the entry point for the console application.
//

#include <gtest/gtest.h>
#include <MainApp.h>

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

TEST(testMainApp, myTest)
{
	ASSERT_EQ(true, true);
}

