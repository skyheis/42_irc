#include "Server.hpp"

void	ft_set_topic(Client &they, Channel &ch, std::string &tmp, std::string &what) {
	if (ch.getMode(MD_T) && !ch._operators.count(they.getNick())) {
		tmp = ":ircap 482 " + they.getNick() + " #" + ch.getName() + " :You're not channel operator\r\n";
		send(they.getFd(), tmp.c_str(), tmp.length(), 0);
	}
	else {
		what.erase(what.begin());
		ch.setTopic(what);
		tmp = ":" + they.getNick() + " TOPIC #" + ch.getName() + " :" + ch.getTopic() + "\r\n";
		
		std::set<int>::const_iterator it = ch._clients.begin();
		std::set<int>::const_iterator end = ch._clients.end();

		while (it != end) {
			send((*it), tmp.c_str(), tmp.length(), 0);
			++it;
		}
	}
}

void	ft_asked_topic(Client &they, Channel &ch, std::string &tmp) {
	if (ch.getTopic().empty())
		tmp = ":ircap 331 " + they.getNick() + " #" + ch.getName() + " :No topic is set\r\n";
	else
		tmp = ":ircap 332 " + they.getNick() + " #" + ch.getName() + " :" + ch.getTopic() + "\r\n";
	send(they.getFd(), tmp.c_str(), tmp.length(), 0);
}

void	Client::topic(t_server &srv) {
	Channel 			*ch;
	std::string			tmp, ch_name, what;
	std::istringstream	buf(srv.command);

	std::getline(buf, tmp, ' ');
	std::getline(buf, ch_name, ' ');
	std::getline(buf, what);

	if (ch_name[0] == '#')
		ch_name.erase(ch_name.begin());
	
	if (!srv.channels.count(ch_name)) {
		tmp = ":ircap 403 " + this->nickname + " #" + ch_name + " :No such channel\r\n";
		send(this->_fd, tmp.c_str(), tmp.length(), 0);
		return ;
	}

	ch = &(srv.channels.find(ch_name)->second);


	if (!ch->_clients.count(this->_fd)) {
		tmp = ":ircap 442 " + this->nickname + " #" + ch_name + " :You're not on that channel\r\n";
		send(this->_fd, tmp.c_str(), tmp.length(), 0);
	}
	else if (what.empty()) {
		ft_asked_topic(*this, *ch, tmp);
	}
	else {
		if (ch->getMode(MD_T) && !ch->_operators.count(this->nickname)) {
			tmp = ":ircap 482 " + this->nickname + " #" + ch_name + " :You're not channel operator\r\n";
			send(this->_fd, tmp.c_str(), tmp.length(), 0);
			return ;
		}
		ft_set_topic(*this, *ch, tmp, what);
	}
}
