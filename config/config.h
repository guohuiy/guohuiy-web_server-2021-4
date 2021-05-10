#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <unistd.h>

using namespace std;

class Config
{
private:
	int port;				//�˿ں�
	int log_write;			//��־д�뷽ʽ
	int trig_mode;			//�������ģʽ
	int listen_trigmode;	//listenfd����ģʽ
	int conn_trigmode;		//connfd����ģʽ
	int opt_linger;			//���Źر�����
	int sql_num;			//���ݿ����ӳ�����
	int thread_num;			//�̳߳��ڵ��߳�����
	int close_log;			//�Ƿ�ر���־
	int actor_model;		//����ģ��ѡ��

	//���ݿ���Ϣ,��¼��,����,����
	string user{ "root" };
	string passwd{ "123456" };
	string databasename{ "yourdb" };

public:
	Config();
	~Config() {};
	
	//geter
	int getPort() { return port; }
	int getLogWrite() { return log_write; }
	int getTrigMode() { return trig_mode; }
	int getListenTrigMode() { return listen_trigmode; }
	int getConnTrigMode() { return conn_trigmode; }
	int getOptLinger() { return opt_linger; }
	int getSqlNum() { return sql_num; }
	int getThreadNum() { return thread_num; }
	int getCloseLog() { return close_log; }
	int getActorModel() { return actor_model; }
	string getSqlUser() { return user; }
	string getSqlPasswd() { return passwd; }
	string getSqlDatabaseName() { return databasename; }

	void parse_arg(int argc, char*argv[]);
};

#endif