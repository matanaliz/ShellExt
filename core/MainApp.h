#pragma once

#include <ThreadPool.h>

#include <vector>
#include <string>

using fileVector = std::vector<std::wstring>;

class MainApp
{
public:
	MainApp(const fileVector&);
	~MainApp();

	MainApp() = delete;
	MainApp(MainApp&) = delete;
	MainApp& operator= (const MainApp&) = delete;

private:
	fileVector m_files;
	ThreadPool m_threadPool;
};