#include "../server/webserver.h"

int main(int argc, char *argv[])
{
	//�����н���
	Config config;
	config.parse_arg(argc, argv);

	WebServer server(config);
	//����
	server.run();

	return 0;
}