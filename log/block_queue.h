/*************************************************************
*ѭ������ʵ�ֵ��������У�m_back = (m_back + 1) % m_max_size;
*�̰߳�ȫ��ÿ������ǰ��Ҫ�ȼӻ���������������ٽ���
**************************************************************/

#ifndef BLOCK_QUEUE_H
#define BLOCK_QUEUE_H

#include <iostream>
#include <cstdlib>
#include <chrono>
#include "../synchronous/synchronous.h"
using namespace std;

template <class T>
class block_queue
{
private:
	std::mutex m_mutex;
	std::condition_variable m_cond;

	T *m_array;

	int m_size;
	int m_max_size;
	int m_front;
	int m_back;

public:
	block_queue(int max_size = 1000)
	{
		if (max_size <= 0)
		{
			exit(-1);
		}

		m_max_size = max_size;
		m_array = new T[max_size];
		m_size = 0;
		m_front = -1;
		m_back = -1;
	}

	void clear()
	{
		std::lock_guard<std::mutex> lg(m_mutex);
		m_size = 0;
		m_front = -1;
		m_back = -1;
	}

	~block_queue()
	{
		std::lock_guard<std::mutex> lg(m_mutex);
		if (m_array != NULL)
			delete[] m_array;
	}

	//�ж϶����Ƿ�����
	bool full()
	{
		std::lock_guard<std::mutex> lg(m_mutex);
		if (m_size >= m_max_size)
		{

			m_mutex.unlock();
			return true;
		}
		return false;
	}
	//�ж϶����Ƿ�Ϊ��
	bool empty()
	{
		std::lock_guard<std::mutex> lg(m_mutex);
		if (0 == m_size)
		{
			return true;
		}

		return false;
	}
	//���ض���Ԫ��
	bool front(T &value)
	{
		std::lock_guard<std::mutex> lg(m_mutex);
		if (0 == m_size)
		{
			return false;
		}
		value = m_array[m_front];

		return true;
	}
	//���ض�βԪ��
	bool back(T &value)
	{
		std::lock_guard<std::mutex> lg(m_mutex);
		if (0 == m_size)
		{
			return false;
		}
		value = m_array[m_back];
		
		return true;
	}

	int size()
	{
		std::lock_guard<std::mutex> lg(m_mutex);
		int tmp = m_size;

		return tmp;
	}

	int max_size()
	{
		std::lock_guard<std::mutex> lg(m_mutex);
		int tmp = m_max_size;

		return tmp;
	}
	//���������Ԫ�أ���Ҫ������ʹ�ö��е��߳��Ȼ���
	//����Ԫ��push������,�൱��������������һ��Ԫ��
	//����ǰû���̵߳ȴ���������,����������
	bool push(const T &item)
	{

		m_mutex.lock();
		if (m_size >= m_max_size)
		{
			m_cond.notify_all();
			m_mutex.unlock();
			return false;
		}

		m_back = (m_back + 1) % m_max_size;
		m_array[m_back] = item;

		m_size++;

		m_cond.notify_all();
		m_mutex.unlock();
		return true;
	}
	//popʱ,�����ǰ����û��Ԫ��,����ȴ���������
	bool pop(T &item)
	{
		{
			std::unique_lock<std::mutex> ul(m_mutex);
			if (m_size <= 0)
			{
				m_cond.wait(ul, [this]() {return this->m_size > 0; });
			}

			m_front = (m_front + 1) % m_max_size;
			item = m_array[m_front];

			m_size--;
		}
		
		return true;
	}

	//�����˳�ʱ����
	bool pop(T &item, int ms_timeout)
	{
		{
			std::unique_lock<std::mutex> ul(m_mutex);
			std::chrono::milliseconds ms(ms_timeout);

			m_cond.wait_for(ul, ms, [this]() {return this->m_size > 0; });

			if (m_size <= 0)
			{
				ul.~unique_lock();
				return false;
			}

			m_front = (m_front + 1) % m_max_size;
			item = m_array[m_front];
			m_size--;
		}
		

		return true;
	}
};

#endif