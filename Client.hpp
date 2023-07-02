#pragma once

#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <map>
#include "sys/socket.h"
#include "Server.hpp"

struct t_server;

class Client
{
	private:
		std::map<std::string, void(Client::*)(t_server &srv)> mappings;
		int const	_fd;
		int			index;
		bool 		_authenticate;
		std::string	username;
		std::string	realname;
		std::string nickname;
		std::string	passwd;
		std::string	buf;
		Client();
	public:
		typedef struct s_server t_server;
		
		Client(int const &fd, int index);

		void	set_authenticate(bool auth);
		bool	get_authenticate() const ;

		void	handleCmd(std::string str, t_server &srv);

		void	setUser(t_server &srv);
		void	setNick(t_server &srv);
		void	joinChannel(t_server &srv);
		// void	kickUser();
		// void	privmsg();
		// void	quit();
		// void	invite();
		// void	topic();
		// void	mode();

		~Client();
};
