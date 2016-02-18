#include "Logger.h"

std::atomic<Logger*> Logger::m_instance;
std::mutex Logger::m_mutex;

Logger::Logger()
{
	m_logFile.open(k_logPath.c_str(), std::fstream::out | std::fstream::app);
}

Logger::~Logger()
{
	if (m_logFile.is_open())
		m_logFile.close();
}

Logger* Logger::GetInstance()
{
	//Threadsafe in C++11
	static Logger instance;
    return &instance;
}

void Logger::LogIt(const std::wstring& str)
{
	//TODO: formated output
	//      sorted log
	std::lock_guard<std::mutex> lock(m_mutex);

	if (m_logFile.is_open())
	{
		//Something wrong with operator<< and whitespaces, noskipws not helping
		//TODO: Try check for '\0' in string
		m_logFile.write(str.c_str(), str.size());
		m_logFile << std::endl;
	}
}