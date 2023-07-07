#pragma once

#include <iostream>
#include <string>
#include "Server.hpp"
#include "Client.hpp"

#define MD_I	0
#define MD_T	1
#define MD_K	2
#define MD_L	3

class	Client;
struct	s_server;

class Channel {
	
	private:

		std::string		_name;
		std::string		_topic;
		std::string		_key;
		bool			_mode[4];
 		
		Channel(void);

	public:
		// std::set<Client*>	_clients;
		// std::set<Client*>	_operators;
		std::set<int>			_clients;
		std::set<std::string>	_operators;
		std::set<std::string>	_invited;


		Channel(std::string const &name);
		Channel(std::string const &name, std::string const &key);
		Channel(Channel const &src);
		~Channel();
		Channel &operator=(Channel const &rhs);

		std::string		getName() const;
		std::string		getTopic() const;
		std::string		getKey() const;
		std::set<int>			getClients() const;
		std::set<std::string>	getOperators() const;
		bool			getMode(int const &mode) const;

		void			setTopic(std::string const &topic);
		void			setKey(std::string const &key);
		void			setMode(int const &mode, bool const &value);
		void			addClient(int client);
		void			addOperator(std::string &client);
		void			removeClient(int client);
		void			removeOperator(std::string &client);
		void			sendMsg(std::string const &msg);
};
