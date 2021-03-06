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
	WorkerThread() = delete;
	WorkerThread(const std::shared_ptr<TaskQueue>& queue);
	// Visual studio cannot into default move ctor
	WorkerThread(WorkerThread&& );
	~WorkerThread();

	WorkerThread(const WorkerThread&) = delete;
	WorkerThread& operator=(WorkerThread&&) = delete;
	WorkerThread& operator=(const WorkerThread&) = delete;

	void start();
	void stop();

private:
	void doWork();
	std::unique_ptr<std::thread> m_thread;
	std::shared_ptr<TaskQueue> m_queue;
	std::atomic<bool> m_run;
};

class ThreadPool
{
public:
	ThreadPool();
	~ThreadPool();

	ThreadPool(const ThreadPool&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;

	void Enqueue(const TaskQueue::type&);
	void Run();

private:

	unsigned int m_hardwareThreads;
	std::shared_ptr<TaskQueue> m_queue;
	std::vector<std::shared_ptr<WorkerThread>> m_threads;
};