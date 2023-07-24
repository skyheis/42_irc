#include "Server.hpp"

void	Client::setNick(t_server &srv)
{
	std::istringstream iss(srv.command);
	std::string command, nick;
	std::getline(iss, command, ' ');
	std::getline(iss, nick, ' ');

	if (nick.empty())
	{
		std::string reply = ":ircap 431 " + this->nickname + " :ircap No nickname given\r\n";
		send(_fd, reply.c_str(), reply.length(), 0);
	}
	else if(srv.nicknames.count(nick))
	{
		std::string out = ":ircap 431 " + this->nickname + " :Nickname is already in use\r\n";
		send(_fd, out.c_str(), out.length(), 0);
		srv.check = false;
		return ;
	}
	else
	{
		std::string reply = ":" + this->nickname + " NICK " + nick + "\r\n";
		send(_fd, reply.c_str(), reply.length(), 0);
		nickname = nick;
		srv.nicknames[nick] = this->_fd;
	}
}

void	ft_set_nick(t_server &srv, int i) {

	srv.client_map[srv.client_fd]->setNick(srv);

	if (nick_used) {
		delete srv.client_map[srv.client_fd];
		srv.client_map.erase(srv.client_fd);
		epoll_ctl(srv.poll_fd, EPOLL_CTL_DEL, srv.client_fd, &srv.ev_lst[i]);
		close(srv.client_fd);
	}

}
