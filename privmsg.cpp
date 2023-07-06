#include "Server.hpp"

void	ft_tochannel(Client &they, t_server &srv, std::string &target, std::string &msg) {
	msg = ":" + they.getNick() + "!" + " PRIVMSG " + target + " " + msg + "\r\n";
	
	target.erase(target.begin());
	std::set<int>::const_iterator it = srv.channels[target]._clients.begin();
	std::set<int>::const_iterator end = srv.channels[target]._clients.end();

	while (it != end) {
		send((*it), msg.c_str(), msg.length(), 0);
		++it;
	}
}

void	ft_oponly(Client &they, t_server &srv, std::string &target, std::string &msg) {
	msg = ":" + they.getNick() + "!" + " PRIVMSG " + target + " " + msg + "\r\n";
	
	target.erase(target.begin());
	std::set<int>::const_iterator it = srv.channels[target]._clients.begin();
	std::set<int>::const_iterator end = srv.channels[target]._clients.end();

	while (it != end) {
		if (find(srv.channels[target]._operators.begin(), srv.channels[target]._operators.end(), srv.client_map[(*it)]->getNick()) !=  srv.channels[target]._operators.end())
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
	std::getline(iss, msg, ' ');
	

	if (target.empty() || msg.empty())
	{
		tmp = "ERROR :No target or message given\r\n";
		send(this->_fd, tmp.c_str(), tmp.length(), 0);
		return ; //error?
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
		return ; //error?
	}
	// else
	// {
	// 	tmp = ":" + nickname + "!" + username + " PRIVMSG " + target + " :" + msg + "\r\n";
	// 	send(this->_fd, tmp.c_str(), tmp.length(), 0);
	// }
}
