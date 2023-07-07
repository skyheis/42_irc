#include "Client.hpp"

Client::Client(int const &fd, int index) : _fd(fd) , index(index) , _authenticate(false)
{
	this->mappings["USER"] = &Client::setUser;
	this->mappings["NICK"] = &Client::setNick;
	this->mappings["JOIN"] = &Client::joinChannel;
	this->mappings["MODE"] = &Client::init_operator;
	// this->mappings["KICK"] = &Client::kickUser;
	this->mappings["PRIVMSG"] = &Client::privmsg;
	// this->mappings["QUIT"] = &Client::quit;
	// this->mappings["INVITE"] = &Client::invite;
	// this->mappings["TOPIC"] = &Client::topic;
}

void		Client::setAuthenticate(bool auth) { _authenticate = auth; }

bool	Client::getAuthenticate() const { return _authenticate; }

void	Client::setOp(bool op) { this->_op = op; }

bool	Client::getOp() const { return (this->_op); }

int			 Client::getFd(void) const { return (this->_fd); }

int			 Client::getIndex(void) const { return (this->index); }

std::string	 Client::getUsername(void) const { return (this->username); }

std::string	 Client::getNick(void) const { return (this->nickname); }

std::string	 Client::getRealname(void) const { return (this->realname); }

std::string	 Client::getPasswd(void) const { return (this->passwd); }

std::string	 Client::getBuf(void) const { return (this->buf); }

void	Client::init_operator(t_server &srv)
{
	(void)srv;
	// /mode #channel +o nickname
	std::istringstream iss(buf);
	std::string command, channel, mode, nickname;
	std::getline(iss, command, ' ');
	std::getline(iss, channel, ' ');
	std::getline(iss, mode, ' ');
	std::getline(iss, nickname, ' ');
	std::string new_nickname = "@" + nickname;

	if (mode != "+o" || nickname.empty())
	{
		std::string reply = "ERROR :No username\r\n";
		send(_fd, reply.c_str(), reply.length(), 0);
		return ;
		//! check if you need to disconnect the client
	}
	else
	{
		std::string reply = "Username and password have been set!\r\n";
		send(_fd, reply.c_str(), reply.length(), 0);
		std::map<std::string, Channel>::iterator it = srv.channels.find(channel);
		if (it != srv.channels.end())
			it->second.addOperator(new_nickname);
		else
		{
			//TODO Handle the situation where the channel doesn't exist in the map
		}
		srv.nicknames[new_nickname] = srv.nicknames[nickname];
		srv.nicknames.erase(nickname);
	}
	//? check if the client is already an operator
	//? change the nickname to be prefixed with an @
	//? check if the client is in the channel
	//? check if you need to send a msg when a clinet becomes an operator
	//? check for the real command 
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
	//! -------------------------------- debugger -------------------------------- */
	// std::cout << "Username: " << username << std::endl;
	// std::cout << "Realname: " << realname << std::endl;
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
	//! ---------------------------------- debug --------------------------------- */
	// std::cout << "Nickname: " << nickname << std::endl;
}

void	Client::handleCmd(std::string str, t_server &srv)
{
	buf = str;

	std::istringstream iss(str);
	std::string command, arg;
	std::getline(iss, command, ' ');
	std::getline(iss, arg, '\0');
	if (command == "mode")
		command = "MODE";

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