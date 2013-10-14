#pragma once

#include <mutex>
#include <atomic>
#include <string>
#include <File.h>

#define LOG_PATH "E:\\log.txt"

class Logger
{
public:
	static Logger* GetInstance();

	void LogIt(const std::wstring& path);
private:
	Logger() {};

	static std::atomic<Logger*> m_instance;
	static std::mutex m_mutex;
};