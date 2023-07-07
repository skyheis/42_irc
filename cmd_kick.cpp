#include "Server.hpp"

bool	ft_kicked_inchannel(Client &they, t_server &srv, Channel *ch, std::string &who) {
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
	// if (sr(this->nickname)
	// lol
	Channel 			*ch;
	std::string			tmp, ch_name, who, comment;
	std::istringstream	buf(srv.command);

	std::getline(buf, tmp, ' ');
	std::getline(buf, ch_name, ' ');
	std::getline(buf, who, ' ');
	std::getline(buf, comment);

	if (comment.empty())
		comment = ":I am Groot!";

	if (!srv.channels.count(ch_name)) {
		//error no channel;
		return ;
	}
	
	ch = &(srv.channels.find(ch_name)->second);

	if (!ch->_clients.count(this->_fd)) {
		// error user not in channel
	}
	else if (!ch->_operators.count(this->nickname)) {
		// error user not op
	}
	else if (!ft_kicked_inchannel(*this, srv, ch, who)) {
		// kicked user not in channel
	}
	else {
		
	}

}
