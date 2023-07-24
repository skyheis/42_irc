#include "Server.hpp"

// std::cout << "Debug: " << *each << std::endl;

void	ft_send_topic(Client &they, Channel &ch) {
	std::string msg = ":ircap 332 " + they.getNick() + " #" + ch.getName() + " :" + ch.getTopic() + "\r\n";

	send(they.getFd(), msg.c_str(), msg.length(), 0);
}

void	ft_send_join_and_lsit(Client &they, Channel &ch, t_server &srv, std::string &ch_name) {
	std::string	tmp, lst_name;
	
	tmp = ":" + they.getNick() + " JOIN #" + ch.getName() + "\r\n";
	lst_name = ":ircap 353 " + they.getNick() + " = #" + ch_name + " :";
	
	for (std::set<int>::const_iterator each = ch._clients.begin(); each != ch._clients.end(); ++each) {
		send((*each), tmp.c_str(), tmp.length(), 0);
		if (ch._operators.count(srv.client_map[(*each)]->getNick()))
			lst_name += "@";
		lst_name += srv.client_map[(*each)]->getNick() + " ";
	}

	lst_name += "\r\n";
	send(they.getFd(), lst_name.c_str(), lst_name.length(), 0);
	lst_name = "ircap 366 " + they.getNick() + " #" + ch_name + " :End of /NAMES list\n\r";	
}

int		ft_checkname(t_server &srv) {
	std::string tmp;

	if (srv.command[5] != '#' && srv.command[5] != '&') {
		tmp = ":" + srv.client_map[srv.client_fd]->getNick() + " wrong join format\r\n";
		send(srv.client_fd, tmp.c_str(), tmp.length(), 0);
		return (1);
	}
	return (0);
}

void	Client::joinChannel(t_server &srv) {

	if (ft_checkname(srv))
		return ;
	Channel 			*ch;
	std::string			tmp, ch_name, key, lst_name;
	std::istringstream	buf(srv.command);
	
	std::getline(buf, tmp, ' ');
	std::getline(buf, ch_name, ' ');
	std::getline(buf, key);
	ch_name.erase(ch_name.begin());
	
	std::map<std::string, Channel>::iterator it = srv.channels.find(ch_name);

	if (it != srv.channels.end() && it->second.getMode(MD_I))
	{
		if (it->second._invited.find(this->nickname) == it->second._invited.end())
		{
			std::string reply = "\033[31m" + this->nickname + " " + ch_name + ":Cannot join channel (+i)\033[0m\r\n";
			send(_fd, reply.c_str(), reply.length(), 0);
			return ;
		}
	}

	if (it != srv.channels.end() && it->second.getMode(MD_K))
	{
		if (it->second._password != key)
		{
			tmp = ":" + this->nickname + " #" + ch_name + " :Cannot join channel (+k)\r\n";
			send(srv.client_fd, tmp.c_str(), tmp.length(), 0);
			return ;  //? the channel is password-protected and the password is wrong
		}
	}

	if (it != srv.channels.end() && it->second.getMode(MD_L))
	{
		ch = &(it->second);
		ch->_count++;
		if (ch->_count > ch->_limit)
		{
			tmp = ":ircserv 471 " + this->nickname + " #" + ch_name + " :Cannot join channel (+l) - channel is full, try again later\r\n";
			send(srv.client_fd, tmp.c_str(), tmp.length(), 0);
			ch->_count--;
			return ;  //? the channel is at full capacity
		}
		ch->_count--;
	}

	if (srv.channels.insert(std::pair<std::string, Channel>(ch_name, Channel(ch_name))).second) {
		ch = &(srv.channels.find(ch_name)->second);
		if (key.length() > 0)
		{
			ch->setKey(key);
			passwordModeOn(*ch);
		}
		ch->addOperator(this->nickname);
	}
	else
	{
		ch = &(srv.channels.find(ch_name)->second);
		if (it->second.getMode(MD_I))
			it->second._invited.erase(this->nickname);
	}

	ch->addClient(srv.client_fd);
	ch->_count++;

	ft_send_join_and_lsit(*this, *ch, srv, ch_name);
	ft_send_topic(*this, *ch);
}
