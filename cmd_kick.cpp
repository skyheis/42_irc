#include "Server.hpp"

void	ft_send_kick(Channel &ch, std::string &tmp) {
	
	for (std::set<int>::const_iterator each = ch._clients.begin(); each != ch._clients.end(); ++each) {
		send((*each), tmp.c_str(), tmp.length(), 0);
	}
}

bool	ft_kicked_inchannel(t_server &srv, Channel *ch, std::string &who) {

	std::set<int>::const_iterator it = ch->_clients.begin();
	std::set<int>::const_iterator end = ch->_clients.end();

	while (it != end) {
		if (!srv.client_map[*(it)]->getNick().compare(who))
			return (true);
		++it;
	}
	return (false);
}

void	Client::kickUser(t_server &srv) {

	Channel 			*ch;
	std::string			tmp, ch_name, who, comment;
	std::istringstream	buf(srv.command);

	std::getline(buf, tmp, ' ');
	std::getline(buf, ch_name, ' ');
	std::getline(buf, who, ' ');
	std::getline(buf, comment);

	if (comment.empty())
		comment = ":I am Groot!";

	if (ch_name[0] == '#' || ch_name[0] == '@')
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
	else if (!ch->_operators.count(this->nickname)) {
		tmp = ":ircap 482 " + this->nickname + " #" + ch_name + " :You're not channel operator\r\n";
		send(this->_fd, tmp.c_str(), tmp.length(), 0);
	}
	else if (!ft_kicked_inchannel(srv, ch, who)) {
		tmp = ":ircap 441 " + this->nickname + " " + who + " #" + ch_name + " :They aren't on that channel\r\n";
		send(this->_fd, tmp.c_str(), tmp.length(), 0);
	}
	else {
		tmp = ":" + this->nickname + " KICK #" + ch_name + " " + who + " " + comment + "\r\n";
		ft_send_kick(*ch, tmp);
		ch->_clients.erase(srv.nicknames[who]);
		ch->_count--;
		ch->_operators.erase(who);
	}

}
