#include "Logger.h"
#include <fstream>

std::atomic<Logger*> Logger::m_instance;
std::mutex Logger::m_mutex;

Logger* Logger::GetInstance()
{
	Logger* tmp = m_instance.load(std::memory_order_relaxed);
    std::atomic_thread_fence(std::memory_order_acquire);
    if (tmp == nullptr) 
	{
        std::lock_guard<std::mutex> lock(m_mutex);
        tmp = m_instance.load(std::memory_order_relaxed);
        if (tmp == nullptr) {
            tmp = new Logger;
            std::atomic_thread_fence(std::memory_order_release);
            m_instance.store(tmp, std::memory_order_relaxed);
        }
    }
    return tmp;
}

void Logger::LogIt(const std::wstring& str)
{
	//Thread problems
	std::wfstream fstr;
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		fstr.open(LOG_PATH, std::fstream::out | std::fstream::app);
		fstr << str << std::endl;
		fstr.close();
	}
}