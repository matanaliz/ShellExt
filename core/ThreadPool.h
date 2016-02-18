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
	std::deque<std::function<unsigned long()>> m_tasks;
	std::condition_variable m_cond;
	bool m_stop;
	unsigned long m_result;
};