/*#include <iostream>
#include <queue>
#include <mutex>
#include<thread>
#include<condition_variable>
#include<chrono>
template <typename T>
class worksqueue {
public:
	//�������еĹ��켰����
	worksqueue() {};
	~worksqueue()
	{
		detele_worksqueue();
	}
	//ʵ����ӣ����Ӽ�ɾ������
	void _enqueue(const T& t);
	bool _dequeue(T& t);
	void detele_worksqueue();
	//ע���ж�Ϊ��Ҳ��Ҫʵ�֣�
	bool _empty();

private:
	std::queue<T> _queue;//ʹ��queueģ��
	//��Ҫ��������ֹͬʱ����
	std::mutex _mtx;
	std::condition_variable cv;
	//ʵ����������

};
*/