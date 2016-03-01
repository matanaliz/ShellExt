#pragma once

#include <atomic>
#include <fstream>
#include <mutex>
#include <string>

#include <File.h>


static const std::string k_defaultLogPath = "E:\\default_log.txt";

class Logger
{
public:
	static Logger* Instance();
	bool SetLogFile(const std::string& path);
	void LogToFile(const std::wstring& wstr);
	void LogToFile(const std::string& str);

private:
	Logger();
	~Logger();

	std::string m_currentPath;
	std::mutex m_mutex;
};