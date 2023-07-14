#include "Server.hpp"
#include <fstream>

void	printClients(t_server &srv) {
	std::ofstream cli_file;
	std::map<int, Client*>::iterator it = srv.client_map.begin();
	
	cli_file.open("clients_debug");
	if (!cli_file.is_open()) {
		std::cerr << "Failed to open file" << std::endl;
		return ;
	}

	while (it != srv.client_map.end()) {
		cli_file << std::endl << "Client fd in map " << it->first << std::endl;
		cli_file << "\tfd: " << it->second->getFd() << std::endl;
		cli_file << "\tindex: " << it->second->getIndex() << std::endl;
		cli_file << "\tauthenticate: " << it->second->getAuthenticate() << std::endl;
		cli_file << "\tusername: " << it->second->getUsername() << std::endl;
		cli_file << "\tnickname: " << it->second->getNick() << std::endl;
		cli_file << "\trealname: " << it->second->getRealname() << std::endl;
		cli_file << "\tpasswd: " << it->second->getPasswd() << std::endl;
		cli_file << "\tbuf: " << it->second->getBuf() << std::endl;
	
		++it;
	}
}

static void	iterClients(std::set<int> cp, std::ofstream &file, t_server &srv) {
	for (std::set<int>::iterator it = cp.begin(); it != cp.end(); ++it)
		file << "\t\t" << srv.client_map[(*it)]->getNick() << std::endl;
}

static void	iterOperators(std::set<std::string> cp, std::ofstream &file) {
	for (std::set<std::string>::iterator it = cp.begin(); it != cp.end(); ++it)
		file << "\t\t" << (*it) << std::endl;
}

void	printChannels(t_server &srv) {
	std::ofstream chan_file;
	std::map<std::string, Channel>::const_iterator it = srv.channels.begin();
	
	chan_file.open("channels_debug");
	if (!chan_file.is_open()) {
		std::cerr << "Failed to open file" << std::endl;
		return ;
	}

	while (it != srv.channels.end()) {
		chan_file << std::endl << "Channel name in map " << it->first << std::endl;
		chan_file << "\tname: " << it->second.getName() << std::endl;
		chan_file << "\ttopic: " << it->second.getTopic() << std::endl;
		chan_file << "\tkey: " << it->second.getKey() << std::endl;
		chan_file << "\tmode i: " << it->second.getMode(MD_I) << std::endl;
		chan_file << "\tmode t: " << it->second.getMode(MD_T) << std::endl;
		chan_file << "\tmode k: " << it->second.getMode(MD_K) << std::endl;
		chan_file << "\tmode l: " << it->second.getMode(MD_L) << std::endl;
		chan_file << "\tclients: " << std::endl;
		iterClients(it->second.getClients(), chan_file, srv);
		chan_file << "\toperators: " << std::endl;
		iterOperators(it->second.getOperators(), chan_file);
		++it;
	}
}
