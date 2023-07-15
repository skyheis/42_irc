#include "Client.hpp"
#include <arpa/inet.h>
#include <fstream>

void	Client::fileTransfer(t_server &srv)
{
	(void)srv;
	std::istringstream iss(buf);
	std::string f_cmd, client, s_cmd, option, file_name, ip, port, file_size;
	std::getline(iss, f_cmd, ' ');
	std::getline(iss, client, ' ');
	std::getline(iss, s_cmd, ' ');
	std::getline(iss, option, ' ');
	std::getline(iss, file_name, ' ');
	std::getline(iss, ip, ' ');
	std::getline(iss, port, ' ');
	std::getline(iss, file_size, '\r');

	if (f_cmd != "PRIVMSG" || s_cmd != ":DCC" || option != "SEND" || srv.nicknames.find(client) == srv.nicknames.end() || file_name.empty() || ip.empty() || port.empty() || file_size.empty())
	{
		std::string reply = "\033[31mERROR :Invalid command!\033[0m\r\n";
		send(_fd, reply.c_str(), reply.length(), 0);
		return ;
	}

	int port_int =  std::strtod(port.c_str(), NULL);
	if (port_int <= 0)
	{
		std::string reply = "\033[31mERROR :Port givin is negative!\033[0m\r\n";
		send(_fd, reply.c_str(), reply.length(), 0);
		return ;
	}
	// if (file_transfer)
	// {
	// 	std::string reply = "\033[31mERROR :TCP connection have already been established!\033[0m\r\n";
	// 	send(_fd, reply.c_str(), reply.length(), 0);
	// 	return ;
	// }
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		std::string reply = "\033[31mERROR :Could not create socket!\033[0m\r\n";
		send(_fd, reply.c_str(), reply.length(), 0);
		return ;
	}
	struct sockaddr_in serv_addr;
	std::memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port_int);
	inet_pton(AF_INET, ip.c_str(), &(serv_addr.sin_addr));

	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		std::string reply = "\033[31mERROR :Connection failure!\033[0m\r\n";
		send(_fd, reply.c_str(), reply.length(), 0);
		return ;
	}
	std::string reply = "\033[32mFile transfer started!\033[0m\r\n";
	send(_fd, reply.c_str(), reply.length(), 0);
	file_transfer = true;

	std::string source_file_path = file_name;
	std::ifstream infile(source_file_path.c_str(), std::ifstream::binary);
	if (!infile)
	{
		std::string reply = "\033[31mERROR :Infile is invalid!\033[0m\r\n";
		send(_fd, reply.c_str(), reply.length(), 0);
		return ;
	}

	const char* home = std::getenv("HOME");
	if (!home)
	{
		std::string reply = "\033[31mERROR :Invalid HOME variable!\033[0m\r\n";
		send(_fd, reply.c_str(), reply.length(), 0);
		return ;
	}
	std::string destination_file_path = std::string(home) + "/" + "_copy";
	std::ofstream outfile(destination_file_path.c_str(), std::ofstream::binary);
	if (!outfile)
	{
		std::string reply = "\033[31mERROR :Outfile wasn't created!\033[0m\r\n";
		send(_fd, reply.c_str(), reply.length(), 0);
		return ;
	}

	char buffer[1024];
	std::streamsize n;
	while ((n = infile.read(buffer, sizeof(buffer)).gcount()) > 0)
	{
		outfile.write(buffer, n);
	}

	std::string newreply = "\033[32mFile transfer completed!\033[0m\r\n";
	send(_fd, newreply.c_str(), newreply.length(), 0);
	infile.close();
	outfile.close();
}