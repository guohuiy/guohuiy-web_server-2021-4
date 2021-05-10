#include <cstdlib>
#include "config.h"

Config::Config():port(9006),log_write(0),
				trig_mode(0),listen_trigmode(0), 
				conn_trigmode(0),opt_linger(0),
				sql_num(8), thread_num(8),
				close_log(0), actor_model(0){
	//�˿ں�,Ĭ��9006
	//��־д�뷽ʽ��Ĭ��ͬ��
	//�������ģʽ,Ĭ��listenfd LT + connfd LT
	//listenfd����ģʽ��Ĭ��LT
	//connfd����ģʽ��Ĭ��LT
	//���Źر����ӣ�Ĭ�ϲ�ʹ��
	//���ݿ����ӳ�����,Ĭ��8
	//�̳߳��ڵ��߳�����,Ĭ��8
	//�ر���־,0Ĭ�ϲ��ر�
	//����ģ��,0Ĭ����proactor
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