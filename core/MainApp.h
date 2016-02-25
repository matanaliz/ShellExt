#pragma once

#include <ThreadPool.h>

#include <vector>
#include <string>

using FileVector = std::vector<std::wstring>;

class MainApp
{
public:
	MainApp(const FileVector&);
	~MainApp() = default;

	MainApp() = delete;
	MainApp(MainApp&) = delete;
	MainApp& operator= (const MainApp&) = delete;

	void Run();

private:
	FileVector m_files;
	ThreadPool m_threadPool;
};