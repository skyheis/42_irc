#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include "sys/socket.h"

class Client
{
	private:
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
		Client(int const &fd, int index);

		void	set_authenticate(bool auth);
		bool	get_authenticate() const ;

		void	handleCmd(std::string str);

		void	setUser();
		// void	setNick();
		// void	joinChannel();
		// void	kickUser();
		// void	privmsg();
		// void	quit();
		// void	invite();
		// void	topic();
		// void	mode();

		~Client();
};