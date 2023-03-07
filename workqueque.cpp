//����һ����������,����Ҫ��������������threadpool�����
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