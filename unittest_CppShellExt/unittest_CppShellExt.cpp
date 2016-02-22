// unittest_CppShellExt.cpp : Defines the entry point for the console application.
//

#include <gtest/gtest.h>
#include <MainApp.h>
#include <ThreadPool.h>

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

TEST(testFile, basicFileTest)
{
	// Make test to fail
	ASSERT_TRUE(false);
}

TEST(testThreadPool, basicThreadPoolTest)
{
	// Make test to fail
	ASSERT_TRUE(false);
}

TEST(testMainApp, basicTest)
{
	// Make test to fail
	ASSERT_TRUE(false);
}

