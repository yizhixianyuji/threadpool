#include <functional>
#include"workqueue.h"
#include <future>

#include <iostream>
#include <queue>
#include <mutex>
#include<thread>
#include<condition_variable>
#include<chrono>
using namespace std;
template<typename T>
class worksqueue {
public:
	//�������еĹ��켰����
	worksqueue() {};
	~worksqueue()
	{
		detele_worksqueue();
	}
	//ʵ����ӣ����Ӽ�ɾ������
	void _enqueue(const T& t) {
		std::unique_lock<mutex> lck(_mtx);
		_queue.push(t);
	}
	bool _dequeue(T& t) {
		std::unique_lock<mutex> lck(_mtx);
		if (_queue.empty() == 1)
		{
			return false;
		}
		else
		{
			t = _queue.front();
			_queue.pop();
			return true;
		}
	}
	void detele_worksqueue() {
		std::unique_lock<mutex> lck(_mtx);
		while (_queue.empty() != 0)
		{
			_queue.pop();
		}
	}
	//ע���ж�Ϊ��Ҳ��Ҫʵ�֣�
	bool _empty() {
		std::unique_lock<mutex> lck(_mtx);
		return _queue.empty();
	}

private:
	std::queue<T> _queue;//ʹ��queueģ��
	//��Ҫ��������ֹͬʱ����
	std::mutex _mtx;
	//ʵ����������
};
using namespace std;
class threadpool {
private:
	std::mutex _mutex;
	std::condition_variable _cv;
	worksqueue<std::function<void()>> _queue;
public:
	//ʵ�ֹ��죬��������
	threadpool(const int n_thread=4):_threads(std::vector<std::thread>(n_thread)),_shutdown(false){};
public:
	class threadwork {
	private:
		int _id;
		threadpool* _pool;
	public:
		threadwork(threadpool* pool, const int id) :_pool(pool), _id(id) {}
		//���һ���º�������ʵ�ַ����߳�
		void operator()() {
			std::function<void()> func;
			bool task;
			while (_pool->_shutdown != 1) {
				std::unique_lock<mutex> lck(_pool->_mutex);
				//�����в�Ϊ��ʱ�����߳�,Ϊ���������߳�
				_pool->_cv.wait(lck, [&] {

					// ���������Ϊ��ʱ��Ҳ���˳��ȴ�
					if (_pool->_shutdown)
						return true;

					return !_pool->_queue._empty();
					});

				task = _pool->_queue._dequeue(func);
				if (task) {
					printf("threadpool [%d]: excuting work... \n", _id);
					func();
					printf("threadpool [%d]: work done. \n", _id);
				}
			}
		}

	};
private:
	//ʵ��һ���̶߳���
	vector<std::thread> _threads;

	std::atomic_bool _shutdown=false;
public:
	//��ʼ���̳߳�
	void init() {
		for (int i = 0; i < 4; i++) {
			_threads[i] = std::thread(threadwork(this, i));
		}
	}
	void shutdown()
	{
		_cv.notify_all();
		for (int i = 0; i < 4; i++)
		{
			if (_threads[i].joinable())
			_threads[i].join();
		}
	}
public:
	template<typename F,typename... Arg>
#if __cplusplis>201103L
	auto sumbit(F&& f,Arg&& ...arg )
#else 
	auto submit(F&& f, Arg&& ...arg)->std::future<decltype(f(arg...))>
#endif
	{
		using result = decltype(f(arg...));
		std::function<result()> func = std::bind(std::forward<F>(f), std::forward<Arg>(arg)...);
		auto task_ptr = std::make_shared<std::packaged_task<result()>>(func);
		std::function<void()> w_func = [task_ptr]() {
			(*task_ptr)();
		};

		_queue._enqueue(w_func);
		_cv.notify_one();
		return task_ptr->get_future();


	}
};

int main() {
	threadpool pools(4);

	pools.init();
	std::this_thread::sleep_for(std::chrono::seconds(1));
	printf("===============================================\n");

	std::vector<std::future<int>> futs;

	for (int i = 0; i < 20; i++)
	{
		printf("submit task %d.\n", i);
		futs.push_back(pools.submit([i] {
			std::this_thread::sleep_for(std::chrono::microseconds(rand() % 500 + 100));
			printf("task %d done!\n", i);
			return i;
			}));
	}

	printf("===============================================\n");
	// �ȴ�10s����ȡ����������
	std::this_thread::sleep_for(std::chrono::seconds(5));
	for (auto& fut : futs) {
		printf("result %d \n", fut.get());
	}

	printf("===============================================\n");
	pools.shutdown();

}
