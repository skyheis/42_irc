#include "Channel.hpp"

Channel::Channel(void) {}

Channel::Channel(std::string const &name) : _name(name) {
	_mode[MD_I] = false;
	_mode[MD_T] = false;
	_mode[MD_K] = false;
	_mode[MD_O] = false;
	_mode[MD_L] = false;
}

Channel::Channel(std::string const &name, std::string const &key) : _name(name), _key(key) {
	_mode[MD_I] = false;
	_mode[MD_T] = false;
	_mode[MD_K] = true;
	_mode[MD_O] = false;
	_mode[MD_L] = false;
}

Channel::Channel(Channel const &src) {
	*this = src;
}

Channel::~Channel() {}

Channel &Channel::operator=(Channel const &rhs) {
	if (this != &rhs) {
		_name = rhs._name;
		_topic = rhs._topic;
		_key = rhs._key;
		_clients = rhs._clients;
		_operators = rhs._operators;
		_mode[MD_I] = rhs._mode[MD_I];
		_mode[MD_T] = rhs._mode[MD_T];
		_mode[MD_K] = rhs._mode[MD_K];
		_mode[MD_O] = rhs._mode[MD_O];
		_mode[MD_L] = rhs._mode[MD_L];
	}
	return *this;
}

std::string		Channel::getName() const {
	return _name;
}

std::string		Channel::getTopic() const {
	return _topic;
}

std::string		Channel::getKey() const {
	return _key;
}

std::vector<Client*>	Channel::getClients() const {
	return _clients;
}

std::vector<Client*>	Channel::getOperators() const {
	return _operators;
}

bool			Channel::getMode(int const &mode) const {
	return _mode[mode];
}

//if is operator
