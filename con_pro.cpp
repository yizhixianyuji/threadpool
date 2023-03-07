/*
//����һ����������
#include <iostream>
#include <queue>
#include <mutex>
#include<thread>
#include<condition_variable>
#include<chrono>
using namespace std;
void sleep1() {
	std::this_thread::sleep_for(chrono::seconds(1));
}
void sleep2() {
	std::this_thread::sleep_for(chrono::seconds(2));
}
template<typename T>
class myworksqueue {
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
		cv.notify_all();
	}
	bool _dequeue(T& t) {
		std::unique_lock<mutex> lck(_mtx);
		if (_queue.empty() == 1)
		{
			cv.wait(lck);
			return false;
		}
		else
		{
			*t=_queue.front();
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
	std::condition_variable cv;
	//ʵ����������
	
};
void provide(void* q) {
	cout << "go" << endl;
	worksqueue<int>* work_p = (worksqueue<int>*)q;
	for (int i = 0; i < 5; i++) {
		work_p->_enqueue(i);
		cout << i<<"������������" << endl;
		sleep2();
	}
}

void consumer(void* q) {
	worksqueue<int>* work_p = (worksqueue<int>*)q;
	while (1) {
		int t=0;
		if (work_p->_dequeue(t) == true)
			cout <<t<< "��ȡ����" << endl;
		else
		{
			cout << "�޲�Ʒ" << endl;
		}
	}

}

int main()
{
	cout<<"test" << endl;
	worksqueue<int> *the_queue=new worksqueue<int>;
	thread t1(provide,the_queue);
	thread t2(consumer, the_queue);
	t1.join();
	t2.join();
}
*/