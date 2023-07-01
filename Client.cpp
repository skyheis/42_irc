#include "Client.hpp"

Client::Client(int const &fd, int index) : _fd(fd) , index(index) , _authenticate(false) {}

void	Client::set_authenticate(bool auth) { _authenticate = auth; }

bool	Client::get_authenticate() const { return _authenticate; }

void	Client::setUser()
{
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

void	Client::handleCmd(std::string str)
{
	buf = str;
	std::map<std::string, void(Client::*)()> mappings;
	mappings["USER"] = &Client::setUser;
	// mappings["NICK"] = &Client::setNick;
	// mappings["JOIN"] = &Client::joinChannel;
	// mappings["KICK"] = &Client::kickUser;
	// mappings["PRIVMSG"] = &Client::privmsg;
	// mappings["QUIT"] = &Client::quit;
	// mappings["INVITE"] = &Client::invite;
	// mappings["TOPIC"] = &Client::topic;
	// mappings["MODE"] = &Client::mode;

	std::istringstream iss(str);
	std::string command, arg;
	std::getline(iss, command, ' ');
	std::getline(iss, arg, '\0');

	std::map<std::string, void(Client::*)()>::iterator it = mappings.find(command);
	if(it != mappings.end())
	{
		std::cout << "\033[32mClient:\033[0m " << index << " " << "\033[34mcommand:\033[0m " << command << " \033[33margs:\033[0m " << arg << std::endl;
		(this->*(it->second))();
	}
	else
		std::cout << "\033[32mClient:\033[0m " << index << " " << "\033[34mcommand:\033[0m " << command << " \033[33margs:\033[0m " << arg << std::endl;
}

Client::~Client() {}