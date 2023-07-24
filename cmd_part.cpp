#include "Server.hpp"

void	ft_send_part(Channel &ch, std::string &tmp) {
	
	for (std::set<int>::const_iterator each = ch._clients.begin(); each != ch._clients.end(); ++each) {
		send((*each), tmp.c_str(), tmp.length(), 0);
	}
}

void	Client::part(t_server &srv) {

	Channel 			*ch;
	std::string			tmp, ch_name, reason;
	std::istringstream	buf(srv.command);

	std::getline(buf, tmp, ' ');
	std::getline(buf, ch_name, ' ');
	std::getline(buf, reason);

	if (!reason.empty())
		reason = " " + reason;

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
	else {
		tmp = ":" + this->nickname + " PART #" + ch_name + reason + "\r\n";
		ft_send_part(*ch, tmp);
		ch->_clients.erase(srv.nicknames[this->nickname]);
		ch->_operators.erase(this->nickname);
		ch->_count--;
	}

}
