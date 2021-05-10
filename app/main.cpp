#include "../server/webserver.h"

int main(int argc, char *argv[])
{
	//ÃüÁîĞĞ½âÎö
	Config config;
	config.parse_arg(argc, argv);

	WebServer server(config);
	//Æô¶¯
	server.run();

	return 0;
}