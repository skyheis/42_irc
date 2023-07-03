#include "Client.hpp"
#include "Channel.hpp"

void	ft_send_string_to_channel(std::map<std::string, Channel>::iterator &it, std::string msg) {
	std::vector<Client *> each = it->second.getClients();
	// std::vector<Client *>::iterator e_each = it->second.getClients().end();
	// std::map<int, Client *>::iterator it = it->second.getClients().begin();

	for (int i = 0; each[i]; i++){
		send(each[i]->getFd(), msg.c_str(), msg.length(), 0);
	}

	// std::cout << "Debug: " << *each << std::endl;
	// for (; each != e_each; each++) {
	// 	// Client *clnt = *each;
	// 	send((*each)->getFd(), msg.c_str(), msg.length(), 0);
	// }
	// for (loco = srv.client_map.begin(); loco != srv.client_map.end(); ++loco)
	// {
	// 	send(loco->second->getFd(), msg.c_str(), msg.length(), 0);
	// }
}

void	ft_send_topic(t_server &srv, std::map<std::string, Channel>::iterator &it) {
	std::string msg = "ircap 332 " + srv.client_map[srv.client_fd]->getNick() + " #" + it->first + " :" + it->second.getTopic() + "\r\n";

	send(srv.client_fd, msg.c_str(), msg.length(), 0);
}

void	ft_send_channel_list(t_server &srv, std::map<std::string, Channel>::iterator &it) {
	// std::vector<Client *>::iterator each = it->second.getClients().begin();
	// std::vector<Client *>::iterator e_each = it->second.getClients().end();
	std::vector<Client *> each = it->second.getClients();
	
	// for (int i = 0; each[i]; i++){
	// 	send(each[i]->getFd(), msg.c_str(), msg.length(), 0);
	// }

	// // std::vector<Client *>::iterator s_ops = it->second.getOperators().begin();
	// std::vector<Client *>::iterator e_ops = it->second.getOperators().end();
	std::string msg = "ircap 353 " + srv.client_map[srv.client_fd]->getNick() + " = #" + it->first + " :";

	for (int i = 0; each[i]; i++){
		// if (std::find(it->second.getOperators().begin(), it->second.getOperators().end(), each) != e_ops)
		// if (std::find(s_ops, e_ops, each) != e_ops)
			// msg += "@";
		msg += each[i]->getNick() + " ";
	}
	msg += "\r\n";
	send(srv.client_fd, msg.c_str(), msg.length(), 0);
	msg = "ircap 366 " + srv.client_map[srv.client_fd]->getNick() + " #" + it->first + " :End of /NAMES list\n\r";
}

void	Client::joinChannel(t_server &srv) {
	std::string tmp, ch_name, key;


	if (srv.buffer[5] != '#' && srv.buffer[5] != '&') {
		tmp = ":" + srv.client_map[srv.client_fd]->getNick() + " wrong join format\r\n";
		send(srv.client_fd, tmp.c_str(), tmp.length(), 0);
		return ; //error but i guess is not possible
	}
	
	std::map<std::string, Channel>::iterator ch_it;
	std::map<int, Client *>::iterator cl_it = srv.client_map.find(srv.client_fd);
	std::istringstream buf(srv.buffer);
	std::getline(buf, tmp, ' ');
	std::getline(buf, ch_name, ' ');
	std::getline(buf, key);

	ch_name.erase(ch_name.begin());
	if (srv.channels.insert(std::pair<std::string, Channel>(ch_name, Channel(ch_name))).second) {
		ch_it = srv.channels.find(ch_name);
		if (key.length() > 0)
			ch_it->second.setKey(key);
		ch_it->second.addOperator(srv.client_map[srv.client_fd]);
	}
	else
		ch_it = srv.channels.find(ch_name);


	if (ch_it->second.getMode(MD_K) && ch_it->second.getKey().compare(key)) {
		tmp = ":" + cl_it->second->getNick() + " #" + ch_name + " :Cannot join channel (+k)\r\n";
		send(srv.client_fd, tmp.c_str(), tmp.length(), 0);
		return ; //send error passwd message
	}

	// ch_it->second.addClient(srv.client_map[srv.client_fd]);
	ch_it->second.addClient(cl_it->second);
	tmp = ":" + cl_it->second->getNick() + " JOIN #" + ch_name + "\r\n";
	// std::cout << "Debug: " << srv.buffer[5] << std::endl;
	ft_send_string_to_channel(ch_it, tmp);
	ft_send_topic(srv, ch_it);
	ft_send_channel_list(srv, ch_it);
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
		