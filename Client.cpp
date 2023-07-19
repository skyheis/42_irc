#include "Client.hpp"

Client::Client(int const &fd, int index) : _fd(fd) , index(index) , _authenticate(false)
{
	this->mappings["USER"] = &Client::setUser;
	this->mappings["NICK"] = &Client::setNick;
	this->mappings["JOIN"] = &Client::joinChannel;
	this->mappings["KICK"] = &Client::kickUser;
	this->mappings["PRIVMSG"] = &Client::privmsg;
	// this->mappings["QUIT"] = &Client::quit;
	this->mappings["INVITE"] = &Client::invite;
	this->mappings["TOPIC"] = &Client::topic;
	this->mappings["PART"] = &Client::part;
}

void		Client::setAuthenticate(bool auth) { _authenticate = auth; }

bool	Client::getAuthenticate() const { return _authenticate; }

void	Client::setOp(bool op) { this->_op = op; }

bool	Client::getOp() const { return (this->_op); }

int			Client::getFd(void) const { return (this->_fd); }

int			Client::getIndex(void) const { return (this->index); }

std::string	Client::getUsername(void) const { return (this->username); }

std::string	Client::getNick(void) const { return (this->nickname); }

std::string	Client::getRealname(void) const { return (this->realname); }

std::string	Client::getPasswd(void) const { return (this->passwd); }

std::string	Client::getBuf(void) const { return (this->buf); }

std::string	Client::getHalfbuf(void) { 
	std::string tmp = this->_halfbuf;
	this->_halfbuf = "";
	return (tmp);
}

void		Client::setHalfbuf(std::string &buf) { this->_halfbuf += buf; }

bool		Client::isHalfbugEmpty() const { return (this->_halfbuf.empty() ? true : false); }

bool	Client::isOpInChannel(std::string channel, t_server &srv) { return (srv.channels[channel].isOperator(this->nickname)); }

void	Client::init_operator(t_server &srv)
{
	(void)srv;
	std::istringstream iss(buf);
	std::string command, channel, mode, cmd_nickname;
	std::getline(iss, command, ' ');
	std::getline(iss, channel, ' ');
	std::getline(iss, mode, ' ');
	std::getline(iss, cmd_nickname, ' ');
	if (channel.empty() || mode.empty())
	{
		std::string reply = "\033[31mERROR :No channel or mode given!\033[0m\r\n";
		send(_fd, reply.c_str(), reply.length(), 0);
		return ;
	}
	if (channel[0] == '#')
		channel.erase(channel.begin());
	if ((mode != "+o" && mode != "-o"))
		return ;
	else if (cmd_nickname.empty())
	{
		std::string reply = "\033[31mERROR :No user was provided!\033[0m\r\n";
		send(_fd, reply.c_str(), reply.length(), 0);
		return ;
	}
	else if (mode == "+o")
	{
		if (cmd_nickname == nickname)
		{
			std::string reply = "\033[31mERROR :You are already an operator!\033[0m\r\n";
			send(_fd, reply.c_str(), reply.length(), 0);
			return ;
		}
		std::map<std::string, Channel>::iterator ito = srv.channels.find(channel);
		if (ito != srv.channels.end())
		{
			if (ito->second._operators.find(cmd_nickname) != ito->second._operators.end())
			{
				std::string reply = "\033[31mERROR :User is already an operator!\033[0m\r\n";
				send(_fd, reply.c_str(), reply.length(), 0);
				return ;
			}
		}
		std::map<std::string, Channel>::iterator it = srv.channels.find(channel);
		if (it != srv.channels.end())
		{
			std::string msg = "\033[32m" + cmd_nickname + " has become an operator in channel: " + channel + "\033[0m\r\n";
			for (std::set<int>::const_iterator iter = srv.channels[channel]._clients.begin(); iter != srv.channels[channel]._clients.end(); ++iter)
				send((*iter), msg.c_str(), msg.length(), 0);
			it->second.addOperator(cmd_nickname);
		}
		else
		{
			std::string reply = "\033[31mERROR :Channel is not found!\033[0m\r\n";
			send(_fd, reply.c_str(), reply.length(), 0);
			return ;
		}
	}
	else if (mode == "-o")
	{
		std::map<std::string, Channel>::iterator it = srv.channels.find(channel);
		if (it != srv.channels.end())
		{
			std::string msg = "\033[32m" + cmd_nickname + " is no longer an operator in channel: " + channel + "\033[0m\r\n";
			for (std::set<int>::const_iterator iter = srv.channels[channel]._clients.begin(); iter != srv.channels[channel]._clients.end(); ++iter)
				send((*iter), msg.c_str(), msg.length(), 0);
			it->second.removeOperator(cmd_nickname);
		}
		else
		{
			std::string reply = "\033[31mERROR :Channel is not found!\033[0m\r\n";
			send(_fd, reply.c_str(), reply.length(), 0);
			return ;
		}
	}
}

void	Client::setUser(t_server &srv)
{
	(void)srv;
	std::istringstream iss(buf);
	std::string command, user, zero, star, real;
	std::getline(iss, command, ' ');
	std::getline(iss, user, ' ');
	std::getline(iss, zero, ' ');
	std::getline(iss, star, ' ');
	std::getline(iss, real, ' ');

	if (user.empty() || zero.empty() || star.empty() || real.empty())
	{
		std::string reply = "ERROR :No username given\r\n";
		send(_fd, reply.c_str(), reply.length(), 0);
	}
	else
	{
		std::string reply = "Username and realname have been set!\r\n";
		send(_fd, reply.c_str(), reply.length(), 0);
		username = user;
		realname = real;
	}
}

void	Client::setNick(t_server &srv)
{
	std::istringstream iss(buf);
	std::string command, nick;
	std::getline(iss, command, ' ');
	std::getline(iss, nick, ' ');

	if (nick.empty())
	{
		std::string reply = "ERROR :No nickname given\r\n";
		send(_fd, reply.c_str(), reply.length(), 0);
	}
	else if(srv.nicknames.count(nick))
	{
		std::string out = "Err_num(431) nickname provided already used\r\n";
		send(_fd, out.c_str(), out.length(), 0);
		std::string dis = "Disconnected ()\r\n";
		send(_fd, dis.c_str(), dis.length(), 0);
		srv.check = false;
		close(srv.client_fd);
		return ;
	}
	else
	{
		std::string reply = "Nickname has been set!\r\n";
		send(_fd, reply.c_str(), reply.length(), 0);
		nickname = nick;
		srv.nicknames[nick] = this->_fd;
	}
}

void	Client::checkOption(t_server &srv)
{
	std::istringstream iss(buf);
	std::string command, chan, option;
	std::getline(iss, command, ' ');
	std::getline(iss, chan, ' ');
	std::getline(iss, option, ' ');
	chan.erase(chan.begin());
	if (chan.empty() || option.empty())
	{
		std::string reply = "\033[31mERROR :No channel or option given\033[0m\r\n";
		send(_fd, reply.c_str(), reply.length(), 0);
		return ;
	}
	if (option == "+k")
		std::getline(iss, srv.channels[chan]._password, ' ');
	else if (option == "+l")
	{
		std::string limit;
		std::getline(iss, limit, ' ');
		srv.channels[chan]._limit = std::strtod(limit.c_str(), NULL);
		if (srv.channels[chan]._limit <= 0)
		{
			std::string reply = "\033[31mERROR :Limit givin is negative!\033[0m\r\n";
			send(_fd, reply.c_str(), reply.length(), 0);
			return ;

		}
		if (srv.channels[chan]._limit < srv.channels[chan]._count)
		{
			std::string reply = "\033[31mERROR :Limit givin is less than the number of users present in the channel!\033[0m\r\n";
			send(_fd, reply.c_str(), reply.length(), 0);
			return ;
		}
	}

	if (srv.channels[chan]._operators.count(nickname))
	{
		if (option == "+o" || option == "-o")
			init_operator(srv);
		else if (option == "+i")
			inviteOnlyModeOn(srv, chan);
		else if (option == "-i")
			inviteOnlyModeOff(srv, chan);
		else if (option == "+t")
			topicModeOn(srv, chan);
		else if (option == "-t")
			topicModeOff(srv, chan);
		else if (option == "+k")
			passwordModeOn(srv, chan);
		else if (option == "-k")
			passwordModeOff(srv, chan);
		else if (option == "+l")
			userLimitModeOn(srv, chan);
		else if (option == "-l")
			userLimitModeOff(srv, chan);
		else
		{
			std::string reply = "\033[31mERROR: Option was not found!\033[0m\r\n";
			send(_fd, reply.c_str(), reply.length(), 0);
			return ;
		}
	}
	else
	{
		std::string reply = "\033[31mERROR: You are not an operator\033[0m\r\n";
		send(_fd, reply.c_str(), reply.length(), 0);
	}
}

void	Client::handleCmd(std::string str, t_server &srv)
{
	buf = str;

	std::istringstream iss(str);
	std::string command, arg;
	std::getline(iss, command, ' ');
	std::getline(iss, arg, '\0');
	if (command == "/mode")
		command = "MODE";
	if (command == "MODE" || command == "mode")
		checkOption(srv);
	std::map<std::string, void(Client::*)(t_server &srv)>::iterator it = this->mappings.find(command);
	if(it != this->mappings.end())
	{
		std::cout << "\033[32mClient:\033[0m " << index << " " << "\033[34mcommand:\033[0m " << command << " \033[33margs:\033[0m " << arg << std::endl;
		(this->*(it->second))(srv);
	}
	else
		std::cout << "\033[32mClient:\033[0m " << index << " " << "\033[34mcommand:\033[0m " << command << " \033[33margs:\033[0m " << arg << std::endl;
}

Client::~Client() {}
