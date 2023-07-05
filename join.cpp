#include "Client.hpp"
#include "Channel.hpp"

// std::cout << "Debug: " << *each << std::endl;

void	ft_send_string_to_channel(Channel &ch, std::string msg) {
	std::set<Client *>::iterator each;
	
	each = ch.getClients().begin();

	for (std::set<Client*>::size_type i = 0; i < ch.getClients().size(); i++, each++) {
		send((*each)->getFd(), msg.c_str(), msg.length(), 0);
	}
}

void	ft_send_topic(Client &they, Channel &ch) {
	std::string msg = "ircap 332 " + they.getNick() + " #" + ch.getName() + " :" + ch.getTopic() + "\r\n";

	send(they.getFd(), msg.c_str(), msg.length(), 0);
}

void	ft_send_channel_list(Client &they, Channel &ch) {
	std::set<Client *>::iterator each;// = ch.getClients().begin();
	each = ch.getClients().begin();

	std::string msg = "ircap 353 " + they.getNick() + " = #" + ch.getName() + " :";

	for (std::set<Client*>::size_type i = 0; i < ch.getClients().size(); i++, each++) {
		// if (std::find(it->second.getOperators().begin(), it->second.getOperators().end(), each) != e_ops)
		// if (std::find(s_ops, e_ops, each) != e_ops)
			// msg += "@";
		msg += (*each)->getNick() + " ";
	}

	msg += "\r\n";
	send(they.getFd(), msg.c_str(), msg.length(), 0);
	msg = "ircap 366 " + they.getNick() + " #" + ch.getName() + " :End of /NAMES list\n\r";
}

void	Client::joinChannel(t_server &srv) {
	std::string tmp, ch_name, key, lst_name;

	if (srv.buffer[5] != '#' && srv.buffer[5] != '&') {
		tmp = ":" + srv.client_map[srv.client_fd]->getNick() + " wrong join format\r\n";
		send(srv.client_fd, tmp.c_str(), tmp.length(), 0);
		return ; //error but i guess is not possible
	}
	
	Channel *ch;
	std::istringstream buf(srv.buffer);
	std::getline(buf, tmp, ' ');
	std::getline(buf, ch_name, ' ');
	std::getline(buf, key);

	ch_name.erase(ch_name.begin());

	if (srv.channels.insert(std::pair<std::string, Channel>(ch_name, Channel(ch_name))).second) {
		ch = &(srv.channels.find(ch_name)->second);
		if (key.length() > 0)
			ch->setKey(key);
		ch->addOperator(srv.client_map[srv.client_fd]);
	}
	else {
		ch = &(srv.channels.find(ch_name)->second);
		if (ch->getMode(MD_K) && ch->getKey().compare(key)) {
			tmp = ":" + this->nickname + " #" + ch_name + " :Cannot join channel (+k)\r\n";
			send(srv.client_fd, tmp.c_str(), tmp.length(), 0);
			return ; //send error passwd message
		}
	}

	ch->addClient(this);

	
	
	tmp = ":" + this->nickname + " JOIN #" + ch->getName() + "\r\n";
	lst_name = "ircap 353 " + this->nickname + " = #" + ch_name + " :";
	
	for (std::set<Client *>::const_iterator each = ch->_clients.begin(); each != ch->_clients.end(); ++each) {
		send((*each)->getFd(), tmp.c_str(), tmp.length(), 0);
		// if (std::find(it->second.getOperators().begin(), it->second.getOperators().end(), each) != e_ops)
		// if (std::find(s_ops, e_ops, each) != e_ops)
			// msg += "@";
		lst_name += (*each)->getNick() + " ";
	}
	
	ft_send_topic(*this, *ch);
	
	lst_name += "\r\n";
	send(this->_fd, lst_name.c_str(), lst_name.length(), 0);
	lst_name = "ircap 366 " + this->nickname + " #" + ch_name + " :End of /NAMES list\n\r";
	
	
	
	// tmp = ":" + this->nickname + " JOIN #" + ch->getName() + "\r\n";
	// for (std::set<Client *>::const_iterator each = ch->_clients.begin(); each != ch->_clients.end(); ++each) {
	// 	send((*each)->getFd(), tmp.c_str(), tmp.length(), 0);
	// }

	// ft_send_channel_list(*this, *ch);

	// each = ch->getClients().begin();


	// for (std::set<Client *>::const_iterator each = ch->_clients.begin(); each != ch->_clients.end(); ++each) {
	// 	// if (std::find(it->second.getOperators().begin(), it->second.getOperators().end(), each) != e_ops)
	// 	// if (std::find(s_ops, e_ops, each) != e_ops)
	// 		// msg += "@";
	// 	tmp += (*each)->getNick() + " ";
	// }

}


	// while (ch_names.find(',') != std::string::npos)
	// {
	// 	// tmp = ch_names;
	// 	// tmp.erase(tmp.begin());
	// 	// tmp.erase(tmp.find(','), tmp.length());
	// 	tmpc = ch_names.substr(0, ch_names.find(','));
	// 	ch_names.erase(0, tmpc.length());
	// 	tmpc.erase(tmpc.begin());
		

	// 	if (srv.channels.find(tmpc) != srv.channels.end()) {
	// 		std::map<std::string, Channel>::iterator it = srv.channels.find(tmpc);

	// 		if (it->second.getMode(MD_K)) {

	// 			tmpk = ch_names.substr(0, keys.find(','));
	// 			keys.erase(0, tmpk.length());
	// 			if (it->second.getKey().compare(tmpk.c_str()))
	// 				continue ; // send channel wrong passwd
	// 		}
	// 		it->second.addClient(srv.client_map[srv.client_fd]);
	// 	}
	// 	else {
	// 		if (keys.length() > 0)

	// 		srv.channels.insert(std::pair<std::string, Channel>(tmpc, Channel(tmpc )));
	// 	}
	// }
		