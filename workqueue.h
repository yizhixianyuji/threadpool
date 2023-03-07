/*#include <iostream>
#include <queue>
#include <mutex>
#include<thread>
#include<condition_variable>
#include<chrono>
template <typename T>
class worksqueue {
public:
	//工作队列的构造及析构
	worksqueue() {};
	~worksqueue()
	{
		detele_worksqueue();
	}
	//实现入队，出队及删除队列
	void _enqueue(const T& t);
	bool _dequeue(T& t);
	void detele_worksqueue();
	//注意判断为空也需要实现，
	bool _empty();

private:
	std::queue<T> _queue;//使用queue模板
	//需要互斥锁防止同时访问
	std::mutex _mtx;
	std::condition_variable cv;
	//实现条件变量

};
*/