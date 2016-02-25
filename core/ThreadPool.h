#pragma once

#include <atomic>
#include <condition_variable>
#include <deque>
#include <thread>
#include <memory>
#include <mutex>
#include <queue>
#include <vector>

template<typename T>
class ThreadsafeQueue
{
public:
	ThreadsafeQueue() = default;
	~ThreadsafeQueue() = default;

	ThreadsafeQueue(const ThreadsafeQueue& other)
	{
		std::lock_guard<std::mutex> lk(other.m_mutex);
		m_data = other.m_data;
	}

	ThreadsafeQueue& operator=(const ThreadsafeQueue&) = delete;
	ThreadsafeQueue& operator=(const ThreadsafeQueue&&) = delete;

	void push(T new_value)
	{
		std::lock_guard<std::mutex> lk(m_mutex);
		m_data.push(new_value);
		m_cond.notify_one();
	}

	bool try_pop(T& value)
	{
		std::lock_guard<std::mutex> lk(m_mutex);
		if (m_data.empty())
			return false;
		value = m_data.front();
		m_data.pop();
		return true;
	}

	std::shared_ptr<T> try_pop()
	{
		std::lock_guard<std::mutex> lk(m_mutex);
		if (m_data.empty())
			return std::shared_ptr<T>();
		std::shared_ptr<T> value(std::make_shared<T>(m_data.front()));
		m_data.pop();
		return value;
	}

	void wait_and_pop(T& value)
	{
		std::unique_lock<std::mutex> lk(m_mutex);
		m_cond.wait(lk, [this]{ return !m_data.empty(); });
		value = m_data.front();
		m_data.pop();
	}

	std::shared_ptr<T> wait_and_pop()
	{
		std::unique_lock<std::mutex> lk(m_mutex);
		m_cond.wait(lk, [this]{ return !m_data.empty(); });
		std::shared_ptr<T> value(std::make_shared<T>(m_data.front()));
		m_data.pop();
		return value;
	}

	bool empty() const
	{
		std::lock_guard<std::mutex> lk(m_mutex);
		return m_data.empty();
	}

	typedef T type;

private:
	mutable std::mutex m_mutex;
	std::condition_variable m_cond;
	std::queue<T> m_data;
};

using TaskQueue = ThreadsafeQueue<std::function<void()>>;

class WorkerThread
{
public:
	WorkerThread(const std::shared_ptr<TaskQueue>& queue)
		: m_thread(nullptr)
		, m_queue(queue)
		, m_run()
	{
		m_run.test_and_set();
		// TODO move thread start to some start() method?
		m_thread = std::move(std::make_unique<std::thread>(
			std::thread(&WorkerThread::doWork, this)));
	}

	WorkerThread() = delete;
	~WorkerThread() { if (m_thread && m_thread->joinable()) m_thread->join(); }

	WorkerThread(const WorkerThread&) = delete;

	// To get vector of WorkerThread to work
	WorkerThread(WorkerThread&& other)
		: m_thread(std::move(other.m_thread))
		, m_queue(std::move(other.m_queue))
		, m_run()
	{ };

	WorkerThread& operator=(const WorkerThread&) = delete;

	void stop() { m_run.clear(); };

private:
	void doWork();
	std::unique_ptr<std::thread> m_thread;
	std::shared_ptr<TaskQueue> m_queue;
	// TODO change this to bool under mutex. Have problems with movector.
	std::atomic_flag m_run;

};

class ThreadPool
{
public:
	ThreadPool();
	~ThreadPool();

	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;


	unsigned long GetResult();

	template<typename FN, typename OBJ>
	void enqueue(FN fn, OBJ o)
	{
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_tasks.push_back(std::function<unsigned long()>(std::bind(fn, o)));
		}

		m_cond.notify_one();
	}

	template<typename FN, typename OBJ>
	void enqueue(FN fn, OBJ o, const unsigned char* buf, size_t size)
	{
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_tasks.push_back(std::function<unsigned long()>(std::bind(fn, o, buf, size)));
		}

		m_cond.notify_one();
	}

private:

	unsigned int m_hardwareThreads;
	std::shared_ptr<TaskQueue> m_queue;
	std::vector<std::shared_ptr<WorkerThread>> m_threads;

	std::mutex m_mutex;
	std::vector<std::thread> m_workers;
	std::deque<std::function<unsigned long()>> m_tasks;
	std::condition_variable m_cond;
	bool m_stop;
	unsigned long m_result;

	class Worker
	{
	public:
		Worker(ThreadPool& p) : m_pool(p) {}
		void operator()();
		Worker& operator=(Worker&) = delete;

	private:
		ThreadPool& m_pool;
	};
};