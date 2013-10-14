#include "ThreadPool.h"

void ThreadPool::Worker::operator()()
{
	std::function<unsigned long()> task;
	while (true)
	{
		{
			std::unique_lock<std::mutex> lock(m_pool.m_mutex);

			while (!m_pool.m_stop && m_pool.m_tasks.empty())
			{
				m_pool.m_cond.wait(lock);
			}

			if (m_pool.m_stop)
				return;

			task = m_pool.m_tasks.front();
			m_pool.m_tasks.pop_front();
		}

		m_pool.m_result += task();
	}
}

ThreadPool::ThreadPool(size_t threads)
	: m_stop(false)
	, m_result(0)
{
	for (size_t i = 0; i < threads; ++i)
		m_workers.push_back(std::thread(Worker(*this)));
}

ThreadPool::~ThreadPool()
{
	if (!m_stop)
	{
		m_stop = true;
		m_cond.notify_all();

		for (auto& it : m_workers)
			it.join();
	}
}

unsigned long ThreadPool::GetResult()
{
	m_stop = true;
	m_cond.notify_all();

	for (auto& it : m_workers)
		it.join();

	return m_result;
}