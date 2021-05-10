#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <unistd.h>

using namespace std;

class Config
{
private:
	int port;				//端口号
	int log_write;			//日志写入方式
	int trig_mode;			//触发组合模式
	int listen_trigmode;	//listenfd触发模式
	int conn_trigmode;		//connfd触发模式
	int opt_linger;			//优雅关闭链接
	int sql_num;			//数据库连接池数量
	int thread_num;			//线程池内的线程数量
	int close_log;			//是否关闭日志
	int actor_model;		//并发模型选择

	//数据库信息,登录名,密码,库名
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