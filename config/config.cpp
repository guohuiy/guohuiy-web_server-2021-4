#include <cstdlib>
#include "config.h"

Config::Config():port(9006),log_write(0),
				trig_mode(0),listen_trigmode(0), 
				conn_trigmode(0),opt_linger(0),
				sql_num(8), thread_num(8),
				close_log(0), actor_model(0){
	//端口号,默认9006
	//日志写入方式，默认同步
	//触发组合模式,默认listenfd LT + connfd LT
	//listenfd触发模式，默认LT
	//connfd触发模式，默认LT
	//优雅关闭链接，默认不使用
	//数据库连接池数量,默认8
	//线程池内的线程数量,默认8
	//关闭日志,0默认不关闭
	//并发模型,0默认是proactor
}

void Config::parse_arg(int argc, char*argv[]) {
	int opt;
	const char *str = "p:l:m:o:s:t:c:a:";
	while ((opt = getopt(argc, argv, str)) != -1)
	{
		switch (opt)
		{
		case 'p':
		{
			port = atoi(optarg);
			break;
		}
		case 'l':
		{
			log_write = atoi(optarg);
			break;
		}
		case 'm':
		{
			trig_mode = atoi(optarg);
			break;
		}
		case 'o':
		{
			opt_linger = atoi(optarg);
			break;
		}
		case 's':
		{
			sql_num = atoi(optarg);
			break;
		}
		case 't':
		{
			thread_num = atoi(optarg);
			break;
		}
		case 'c':
		{
			close_log = atoi(optarg);
			break;
		}
		case 'a':
		{
			actor_model = atoi(optarg);
			break;
		}
		default:
			break;
		}
	}
}