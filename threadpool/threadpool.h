#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "../synchronous/synchronous.h"
#include "../sql/mysql/sql_connection_pool.h"

#include <list>
#include <vector>
#include <cstdio>
#include <exception>
#include <thread>

template <typename T>
class threadpool
{
private:
	int m_thread_number;        //�̳߳��е��߳���
	int m_max_requests;         //�����������������������
	std::shared_ptr<std::vector<std::shared_ptr<std::thread>>> m_threads;
	std::list<T *> m_workqueue; //�������
	std::mutex m_queuelocker;       //����������еĻ�����
	sem m_queuestat;            //�Ƿ���������Ҫ����
	connection_pool *m_connPool;  //���ݿ�
	int m_actor_model;          //ģ���л�

	/*�����߳����еĺ����������ϴӹ���������ȡ������ִ��֮*/
	static void *worker(void *arg);
	void run();

public:
	/*thread_number���̳߳����̵߳�������max_requests������������������ġ��ȴ���������������*/
	threadpool(int actor_model, connection_pool *connPool, int thread_number = 8, int max_request = 10000);
	~threadpool();
	bool append(T *request, int state);
	bool append_p(T *request);
};

template <typename T>
threadpool<T>::threadpool(int actor_model, connection_pool *connPool, int thread_number, int max_requests) :
	m_actor_model(actor_model), m_thread_number(thread_number),
	m_max_requests(max_requests), m_threads(NULL), m_connPool(connPool)
{
	if (thread_number <= 0 || max_requests <= 0)
		throw std::exception();
	try {
		m_threads.reset(new std::vector< std::shared_ptr<std::thread>>(m_thread_number));
		
		for (int i = 0; i < thread_number; ++i)
		{
			m_threads->at(i).reset(new std::thread(worker, this));
			m_threads->at(i)->detach();
		}
	}
	catch (const std::exception &e) {
		throw e;
	}

}
template <typename T>
threadpool<T>::~threadpool()
{
	//delete[] m_threads;
}
template <typename T>
bool threadpool<T>::append(T *request, int state)
{
	{
		std::lock_guard<std::mutex> lg(m_queuelocker);
		if (m_workqueue.size() >= m_max_requests)
		{
			lg.~lock_guard();
			return false;
		}
		request->m_state = state;
		m_workqueue.push_back(request);
	}

	m_queuestat.post();
	return true;
}
template <typename T>
bool threadpool<T>::append_p(T *request)
{
	{
		std::lock_guard<std::mutex> lg(m_queuelocker);

		if (m_workqueue.size() >= m_max_requests)
		{
			return false;
		}
		m_workqueue.push_back(request);
	}

	m_queuestat.post();
	return true;
}
template <typename T>
void *threadpool<T>::worker(void *arg)
{
	threadpool *pool = (threadpool *)arg;
	pool->run();
	return pool;
}
template <typename T>
void threadpool<T>::run()
{
	while (true)
	{
		T *request = nullptr;
		m_queuestat.wait();
		{
			std::lock_guard<std::mutex> lg(m_queuelocker);
			if (m_workqueue.empty())
			{
				continue;
			}

			request = m_workqueue.front();
			m_workqueue.pop_front();
		}

		if (!request)
		{
			continue;
		}

		if (1 == m_actor_model)
		{
			if (0 == request->m_state)
			{
				if (request->read_once())
				{
					request->improv = 1;
					connectionRAII mysqlcon(&request->mysql, m_connPool);
					request->process();
				}
				else
				{
					request->improv = 1;
					request->timer_flag = 1;
				}
			}
			else
			{
				if (request->write())
				{
					request->improv = 1;
				}
				else
				{
					request->improv = 1;
					request->timer_flag = 1;
				}
			}
		}
		else
		{
			connectionRAII mysqlcon(&request->mysql, m_connPool);
			request->process();
		}
	}
}
#endif
