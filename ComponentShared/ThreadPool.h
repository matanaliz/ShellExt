#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <deque>

class ThreadPool
{
public:
	ThreadPool(size_t);
	~ThreadPool();

	template<typename FN, class C>
	void enqueue(FN fn, C c)
	{
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_tasks.push_back(std::function<void()>(std::bind(fn, c)));
		}

		m_cond.notify_one();
	}

	static const unsigned int THREAD_NUMBER = 8;

private:
	class Worker
	{
	public:
		Worker(ThreadPool& p) : m_pool(p) {}
		void operator()();

	private:
		ThreadPool& m_pool;
	};

	std::mutex m_mutex;
	std::vector<std::thread> m_workers;
	std::deque<std::function<void()>> m_tasks;
	std::condition_variable m_cond;
	bool stop;
};