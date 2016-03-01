// unittest_CppShellExt.cpp : Defines the entry point for the console application.
//

#include <gtest/gtest.h>
#include <MainApp.h>
#include <ThreadPool.h>
#include <Logger.h>

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

TEST(testFile, basicLoggerEmptyFileTest)
{
	// Fail to set empty path
	bool res = Logger::Instance()->SetLogFile("");
	ASSERT_FALSE(res);
}

TEST(testFile, basicLoggerTest)
{
	// Fail to set empty path
	bool res = Logger::Instance()->SetLogFile("test.txt");
	Logger::Instance()->LogToFile("test");
	ASSERT_FALSE(res);
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

