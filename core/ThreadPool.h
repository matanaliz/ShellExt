#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <deque>
#include <memory>

#include <queue>

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

private:
	mutable std::mutex m_mutex;
	std::condition_variable m_cond;
	std::queue<T> m_data;
};

class WorkerThread
{
public:
	WorkerThread();
	~WorkerThread() = default;

	WorkerThread(WorkerThread&) = delete;
	WorkerThread& operator=(const WorkerThread&) = delete;

	void start();
	void stop();

private:
	

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
	ThreadsafeQueue<std::function<void()>> m_queue;
	std::vector<WorkerThread> m_threads; //Rename this shit

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