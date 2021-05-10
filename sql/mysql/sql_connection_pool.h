#ifndef _CONNECTION_POOL_
#define _CONNECTION_POOL_

#include <stdio.h>
#include <list>
#include <mysql/mysql.h>
#include <error.h>
#include <string.h>
#include <iostream>
#include <string>

#include "../../synchronous/synchronous.h"
#include "sql_connection_pool.h"

using namespace std;

class connection_pool
{
private:
	int m_MaxConn;			//���������
	int m_CurConn;			//��ǰ��ʹ�õ�������
	int m_FreeConn;			//��ǰ���е�������
	std::mutex lock;
	list<MYSQL *> connList; //���ӳ�
	std::shared_ptr<sem> reserve;

	//SQL
	string m_url;			//������ַ
	string m_Port;			//���ݿ�˿ں�
	string m_User;			//��½���ݿ��û���
	string m_PassWord;		//��½���ݿ�����
	string m_DatabaseName;  //ʹ�����ݿ���
	int m_close_log;		//��־����

	connection_pool();
	~connection_pool();

public:
	MYSQL *GetConnection();				 //��ȡ���ݿ�����
	bool ReleaseConnection(MYSQL *conn); //�ͷ�����
	int GetFreeConn();					 //��ȡ����
	void DestroyPool();					 //������������

	//����ģʽ
	static connection_pool *GetInstance();

	void init(string url, string User, string PassWord, string DataBaseName, int Port, int MaxConn, int close_log);
};

class connectionRAII 
{
private:
	MYSQL *conRAII;
	connection_pool *poolRAII;

public:
	connectionRAII(MYSQL **con, connection_pool *connPool);
	~connectionRAII();
};

#endif
