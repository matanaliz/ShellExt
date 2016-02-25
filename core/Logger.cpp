#include <iostream>

#include "Logger.h"

Logger::Logger()
	: m_currentPath(k_defaultLogPath)
	, m_mutex()
{
}

Logger::~Logger()
{
}

bool Logger::SetLogFile(const std::string& path)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	bool res = false;
	std::ofstream fs;
	try
	{
		fs.open(path.c_str(), std::fstream::out | std::fstream::app);
		if (fs.is_open())
		{
			m_currentPath = path;
			res = true;
		}
	}
	catch (std::ofstream::failure& e)
	{
		std::cerr << e.what();
	}
	
	return res;
}

Logger* Logger::Instance()
{
	//Threadsafe in C++11
	static Logger instance;
    return &instance;
}

void Logger::LogToFile(const std::wstring& str)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	std::wofstream fs;
	try
	{
		fs.open(m_currentPath.c_str(), std::fstream::out | std::fstream::app);
		if (fs.is_open())
		{
			fs << str << std::endl;
			fs.close();
		}
	}
	catch (std::ofstream::failure& e)
	{
		std::cerr << e.what();
	}
}

void Logger::LogToFile(const std::string& str)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	std::ofstream fs;
	try
	{
		fs.open(m_currentPath.c_str(), std::fstream::out | std::fstream::app);
		if (fs.is_open())
		{
			fs << str << std::endl;
			fs.close();
		}
	}
	catch (std::ofstream::failure& e)
	{
		std::cerr << e.what();
	}
}