#pragma once

#include <atomic>
#include <fstream>
#include <mutex>
#include <string>

#include <File.h>


static const std::string k_logPath = "D:\\log.txt";

class Logger
{
public:
	static Logger* GetInstance();

	void LogIt(const std::wstring& path);
private:
	Logger();
	~Logger();

	static std::atomic<Logger*> m_instance;
	static std::mutex m_mutex;
	std::wfstream m_logFile;
};