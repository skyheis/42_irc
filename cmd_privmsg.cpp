#include "Server.hpp"

bool	ft_is_inchannel(Channel &ch, int fd_cli) {
	if (ch._clients.count(fd_cli))
		return (true);
	return (false);
}

void	ft_tochannel(Client &they, t_server &srv, std::string &target, std::string &msg) {
	
	target.erase(target.begin());
	
	if (!srv.channels.count(target)) {
		msg = ":ircap 403 " + they.getNick() + " #" + target + " :No such channel\r\n";
		send(they.getFd(), msg.c_str(), msg.length(), 0);
		return ;
	}
	
	Channel *ch = &(srv.channels.find(target)->second);

	if (!ft_is_inchannel(*ch, they.getFd())) {
		msg = ":ircap 442 " + they.getNick() + " #" + target + " :You're not on that channel\r\n";
		send(they.getFd(), msg.c_str(), msg.length(), 0);
		return ;
	}

	msg = ":" + they.getNick() + "!" + " PRIVMSG #" + target + " " + msg + "\r\n";

	std::set<int>::const_iterator it = ch->_clients.begin();
	std::set<int>::const_iterator end = ch->_clients.end();

	while (it != end) {
		if ((*it) != they.getFd())
			send((*it), msg.c_str(), msg.length(), 0);
		++it;
	}
}

void	ft_oponly(Client &they, t_server &srv, std::string &target, std::string &msg) {
	
	target.erase(target.begin());

	if (!srv.channels.count(target)) {
		msg = ":ircap 403 " + they.getNick() + " #" + target + " :No such channel\r\n";
		send(they.getFd(), msg.c_str(), msg.length(), 0);
		return ;
	}
	
	Channel *ch = &(srv.channels.find(target)->second);
	
	if (!ft_is_inchannel(*ch, they.getFd())) {
		msg = ":ircap 442 " + they.getNick() + " #" + target + " :You're not on that channel\r\n";
		send(they.getFd(), msg.c_str(), msg.length(), 0);
		return ;
	}

	msg = ":" + they.getNick() + "!" + " PRIVMSG #" + target + " " + msg + "\r\n";
	
	std::set<int>::const_iterator it = ch->_clients.begin();
	std::set<int>::const_iterator end = ch->_clients.end();

	while (it != end) {
		if (ch->_operators.count(srv.client_map[(*it)]->getNick()) && (*it) != they.getFd())
			send((*it), msg.c_str(), msg.length(), 0);
		++it;
	}
}

void	ft_touser(Client &they, t_server &srv, std::string &target, std::string &msg) {
	msg = ":" + they.getNick() + "!" + " PRIVMSG " + target + " " + msg + "\r\n";

	send(srv.nicknames[target], msg.c_str(), msg.length(), 0);
}

void	Client::privmsg(t_server &srv)
{
	std::istringstream iss(buf);
	std::string tmp, target, msg;
	std::getline(iss, tmp, ' ');
	std::getline(iss, target, ' ');
	std::getline(iss, msg, '\r');

	if (msg.find(":DCC") != std::string::npos)
	{
		fileTransfer(srv);
		return ;
	}
	
	if (target.empty() || msg.empty())
	{
		tmp = "ERROR :No target or message given\r\n";
		send(this->_fd, tmp.c_str(), tmp.length(), 0);
		return ;
	}

	if (target[0] == '@' && target[1] == '#')
		ft_oponly(*this, srv, target, msg);
	else if (target[0] == '#')
		ft_tochannel(*this, srv, target, msg);
	else if (target[0] != '@')
		ft_touser(*this, srv, target, msg);
	else {
		tmp = "ERROR :No target or message given\r\n";
		send(this->_fd, tmp.c_str(), tmp.length(), 0);
		return ;
	}
}
