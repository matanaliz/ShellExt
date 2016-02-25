#include "ThreadPool.h"

static unsigned int kDefaultThreads = 2;

WorkerThread::WorkerThread(const std::shared_ptr<TaskQueue>& queue)
	: m_thread(nullptr)
	, m_queue(queue)
	, m_run(false)
{
}

WorkerThread::WorkerThread(WorkerThread&& other)
	: m_thread(std::move(other.m_thread))
	, m_queue(std::move(other.m_queue))
	, m_run(other.m_run.load())
{
}

WorkerThread::~WorkerThread()
{
	if (m_thread && m_thread->joinable()) 
		m_thread->join();
}

void WorkerThread::start()
{
	m_run.store(true);
	m_thread = std::move(std::make_unique<std::thread>(
		std::thread(&WorkerThread::doWork, this)));
}

void WorkerThread::stop()
{
	m_run.store(false);
}

void WorkerThread::doWork()
{
	// This will be busy-wait. Should use wait_and_pop.
	while (m_run.load())
	{
		try
		{
			TaskQueue::type task;
			if (m_queue->try_pop(task))
			{
				task();
				// TODO save execution results in queue, if function has return type
			}
		}
		catch (...)
		{
			// Silently finish work. Thread will be joined on dtor
			// TODO Pass exception to thread pool
			return;
		}
	}
}

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

		std::unique_lock<std::mutex> lock(m_pool.m_mutex);
		m_pool.m_result += task();
	}
}

ThreadPool::ThreadPool()
	: m_hardwareThreads(0)
	, m_queue(std::make_shared<TaskQueue>(TaskQueue()))
	, m_threads()
	, m_stop(false)
	, m_result(0)
{
	// We will spawn n-1 threads for our threadpool
	m_hardwareThreads = std::thread::hardware_concurrency() - 1;
	if (0 == m_hardwareThreads)
	{
		// We failed to get correct hardware concurency.
		m_hardwareThreads = kDefaultThreads;
	}

	for (size_t i = 0; i < m_hardwareThreads; ++i)
		m_threads.push_back(std::make_shared<WorkerThread>(WorkerThread(m_queue)));

}

ThreadPool::~ThreadPool()
{
	for (const auto& worker : m_threads)
	{
		worker->stop();
	}
	m_threads.clear();

	//if (!m_stop)
	//{
	//	m_stop = true;
	//	m_cond.notify_all();

	//	for (auto& it : m_workers)
	//		it.join();
	//}
}

unsigned long ThreadPool::GetResult()
{
	m_stop = true;
	m_cond.notify_all();

	for (auto& it : m_workers)
		it.join();

	return m_result;
}