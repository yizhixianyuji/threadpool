//创建一个工作队列,不需要考虑条件变量，threadpool会出手
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
	//工作队列的构造及析构
	worksqueue() {};
	~worksqueue()
	{
		detele_worksqueue();
	}
	//实现入队，出队及删除队列
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
	//注意判断为空也需要实现，
	bool _empty() {
		std::unique_lock<mutex> lck(_mtx);
		return _queue.empty();
	}

private:
	std::queue<T> _queue;//使用queue模板
	//需要互斥锁防止同时访问
	std::mutex _mtx;
	//实现条件变量
};