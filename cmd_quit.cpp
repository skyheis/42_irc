#include "Server.hpp"

void	ft_quit_tochannel(Channel *ch, std::string &msg) {
	
	std::set<int>::const_iterator it = ch->_clients.begin();
	std::set<int>::const_iterator end = ch->_clients.end();

	while (it != end) {
		send((*it), msg.c_str(), msg.length(), 0);
		++it;
	}
}

void	Client::quit(t_server &srv) {

	Channel *ch;
	std::string			tmp, reason;
	std::istringstream	buf(srv.command);
	
	std::getline(buf, tmp, ' ');
	std::getline(buf, reason);

	if (reason.length() < 2) {
		reason = ":kthxbyee";
	}
	
	tmp = ":" + this->nickname + " QUIT " + reason + "\r\n";

	std::map<std::string, Channel>::iterator it = srv.channels.begin();

	while (it != srv.channels.end()) {
		if (it->second._clients.count(this->_fd)) {
			ch = &(it->second);
			ch->_count--;
			ch->_clients.erase(this->_fd);
			ch->_operators.erase(this->nickname);
			ft_quit_tochannel(ch, tmp);
		}
		++it;
	}

	srv.nicknames.erase(this->nickname);
}

void	ft_client_quit(t_server &srv, int i) {
	std::cerr << "Connection closed by the client with QUIT cmd" << std::endl;

	srv.client_map[srv.client_fd]->quit(srv);
	delete srv.client_map[srv.client_fd];
	srv.client_map.erase(srv.client_fd);
	epoll_ctl(srv.poll_fd, EPOLL_CTL_DEL, srv.client_fd, &srv.ev_lst[i]);
	close(srv.client_fd);
}
