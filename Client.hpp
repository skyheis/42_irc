#pragma once

#include <algorithm>
#include <string>
#include <sstream>
#include <map>
#include "sys/socket.h"
#include "Server.hpp"
// #include <cstddef>

typedef struct s_server t_server;

class Client
{
	private:
		std::map<std::string, void(Client::*)(t_server &srv)> mappings;
		int			_fd;
		int			index;
		bool 		_authenticate;
		bool		_op;
		std::string	username;
		std::string	realname;
		std::string nickname;
		std::string	passwd;
		std::string	buf;
		std::string	_halfbuf;
		Client();
	public:
		
		Client(int const &fd, int index);

		void	setAuthenticate(bool auth);
		bool	getAuthenticate() const ;
		void	setOp(bool op);
		bool	getOp() const ;


		void	handleCmd(std::string str, t_server &srv);

		int			getFd(void) const;
		int			getIndex(void) const;
		std::string	getUsername(void) const;
		std::string	getNick(void) const;
		std::string	getRealname(void) const;
		std::string	getPasswd(void) const;
		std::string	getBuf(void) const;
		bool		isOpInChannel(std::string channel, t_server &srv);

		std::string	getHalfbuf(void);
		void		setHalfbuf(std::string &buf);
		bool		isHalfbugEmpty() const;

		void	setUser(t_server &srv);
		void	setNick(t_server &srv);
		void	joinChannel(t_server &srv);
		void	init_operator(t_server &srv);
		void	kickUser(t_server &srv);
		void	privmsg(t_server &srv);
		void	quit(t_server &srv);
		void	invite(t_server &srv);
		void	topic(t_server &srv);
		void	part(t_server &srv);
		// void	mode();

		void	inviteOnlyModeOn(t_server &srv, std::string chan);
		void	inviteOnlyModeOff(t_server &srv, std::string chan);
		void	topicModeOn(t_server &srv, std::string chan);
		void	topicModeOff(t_server &srv, std::string chan);
		void	passwordModeOn(t_server &srv, std::string chan);
		void	passwordModeOff(t_server &srv, std::string chan);
		void	userLimitModeOn(t_server &srv, std::string chan);
		void	userLimitModeOff(t_server &srv, std::string chan);

		void			checkOption(t_server &srv);

		~Client();
};
