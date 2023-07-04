#pragma once

#include <iostream>
#include <string>
#include "Server.hpp"
#include "Client.hpp"

#define MD_I	0
#define MD_T	1
#define MD_K	2
#define MD_O	3
#define MD_L	4

class	Client;
struct	s_server;

class Channel {
	
	private:

		std::string		_name;
		std::string		_topic;
		std::string		_key;
		bool			_mode[5];
 		
		Channel(void);

	public:
		std::set<Client*>	_clients;
		std::set<Client*>	_operators;

		Channel(std::string const &name);
		Channel(std::string const &name, std::string const &key);
		Channel(Channel const &src);
		~Channel();
		Channel &operator=(Channel const &rhs);

		std::string		getName() const;
		std::string		getTopic() const;
		std::string		getKey() const;
		std::set<Client*>	getClients() const;
		std::set<Client*>	getOperators() const;
		bool			getMode(int const &mode) const;

		void			setTopic(std::string const &topic);
		void			setKey(std::string const &key);
		void			setMode(int const &mode, bool const &value);
		void			addClient(Client *client);
		void			addOperator(Client *client);
		void			removeClient(Client *client);
		void			removeOperator(Client *client);
		void			sendMsg(std::string const &msg);

};
