#include "ThreadPool.h"

void ThreadPool::Worker::operator()()
{
	std::function<void()> task;
	while (true)
	{
		{
			std::unique_lock<std::mutex> lock(m_pool.m_mutex);

			while (!m_pool.stop && m_pool.m_tasks.empty())
			{
				m_pool.m_cond.wait(lock);
			}

			if (m_pool.stop && m_pool.m_tasks.empty())
				return;

			task = m_pool.m_tasks.front();
			m_pool.m_tasks.pop_front();
		}

		task();
	}
}

ThreadPool::ThreadPool(size_t threads)
	: stop(false)
{
	for (size_t i = 0; i < threads; ++i)
		m_workers.push_back(std::thread(Worker(*this)));
}

ThreadPool::~ThreadPool()
{
	stop = true;
	m_cond.notify_all();

	for (auto& it : m_workers)
		it.join();
}