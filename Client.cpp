#include "Client.hpp"

Client::Client(int const &fd, int index) : _fd(fd) , index(index) , _authenticate(false) {
	this->mappings["USER"] = &Client::setUser;
	this->mappings["NICK"] = &Client::setNick;
	this->mappings["JOIN"] = &Client::joinChannel;
	// this->mappings["KICK"] = &Client::kickUser;
	this->mappings["PRIVMSG"] = &Client::privmsg;
	// this->mappings["QUIT"] = &Client::quit;
	// this->mappings["INVITE"] = &Client::invite;
	// this->mappings["TOPIC"] = &Client::topic;
	// this->mappings["MODE"] = &Client::mode;
}

void	Client::setAuthenticate(bool auth) { _authenticate = auth; }

bool	Client::getAuthenticate() const { return _authenticate; }

int			 Client::getFd(void) const { return (this->_fd); }

int			 Client::getIndex(void) const { return (this->index); }

std::string	 Client::getUsername(void) const { return (this->username); }

std::string	 Client::getNick(void) const { return (this->nickname); }

std::string	 Client::getRealname(void) const { return (this->realname); }

std::string	 Client::getPasswd(void) const { return (this->passwd); }

std::string	 Client::getBuf(void) const { return (this->buf); }


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
	else if(std::find(srv.nicknames.begin(), srv.nicknames.end(), nick) != srv.nicknames.end())
	{
		std::cout << "Err_num(431) nickname provided already used" << std::endl;
		return ;
	}
	else
	{
		std::string reply = "Nickname has been set!\r\n";
		send(_fd, reply.c_str(), reply.length(), 0);
		nickname = nick;
		srv.nicknames.push_back(nick);
	}
	std::cout << "Nickname: " << nickname << std::endl;
}

void	Client::handleCmd(std::string str, t_server &srv)
{
	buf = str;

	std::istringstream iss(str);
	std::string command, arg;
	std::getline(iss, command, ' ');
	std::getline(iss, arg, '\0');

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

