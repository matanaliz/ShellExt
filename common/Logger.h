#pragma once

#include <mutex>
#include <atomic>
#include <string>
#include <File.h>
#include <fstream>

static const std::string k_logPath = "E:\\log.txt";

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