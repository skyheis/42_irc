#pragma once

#include <algorithm>
#include <string>
#include <sstream>
#include <map>
#include "sys/socket.h"
#include "Server.hpp"

typedef struct s_server t_server;

class Client
{
	private:
		// std::map<std::string, void(Client::*)(t_server &srv)> mappings;
		int			_fd;
		int			index;
		bool 		_authenticate;
		std::string	username;
		std::string	realname;
		std::string nickname;
		std::string	passwd;
		std::string	buf;
		Client();
	public:
		
		Client(int const &fd, int index);

		void	set_authenticate(bool auth);
		bool	get_authenticate() const ;

		void	handleCmd(std::string str, t_server &srv);

		std::string	 getNick(void) const;
		int			 getFd(void) const;

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
