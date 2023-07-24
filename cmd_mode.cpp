#include "Server.hpp"

void	Client::checkOption(t_server &srv)
{
	Channel *ch;
	std::istringstream iss(buf);
	std::string command, chan, option;
	std::getline(iss, command, ' ');
	std::getline(iss, chan, ' ');
	std::getline(iss, option, ' ');

	if (chan.empty())
	{
		std::string reply = ":ircap 300 MODE :Not enough parameters\r\n"; 
		send(_fd, reply.c_str(), reply.length(), 0);
		return ;
	}
	else if (option.empty())
		return ; //RPL_CHANNELMODEIS (324) 

	if (chan[0] == '#')
		chan.erase(chan.begin());
	
	if (!srv.channels.count(chan)) {
		command = ":ircap 403 " + this->nickname + " #" + chan + " :No such channel\r\n";
		send(this->_fd, command.c_str(), command.length(), 0);
		return ;
	}

	ch = &(srv.channels.find(chan)->second);

	if (option == "+k")
		std::getline(iss, ch->_password, ' ');
	else if (option == "+l")
	{
		std::string limit;
		std::getline(iss, limit, ' ');
		ch->_limit = std::strtod(limit.c_str(), NULL);
		if (ch->_limit <= 0)
		{
			std::string reply = ":ircap 300 ERROR :Limit givin is negative!\r\n";
			send(_fd, reply.c_str(), reply.length(), 0);
			return ;

		}
		if (ch->_limit < ch->_count)
		{
			std::string reply = ":ircap 300 ERROR :Limit givin is less than the number of users present in the channel!\r\n";
			send(_fd, reply.c_str(), reply.length(), 0);
			return ;
		}
	}

	if (ch->_operators.count(nickname))
	{
		if (option == "+o" || option == "-o")
			init_operator(srv);
		else if (option == "+i")
			inviteOnlyModeOn(*ch);
		else if (option == "-i")
			inviteOnlyModeOff(*ch);
		else if (option == "+t")
			topicModeOn(*ch);
		else if (option == "-t")
			topicModeOff(*ch);
		else if (option == "+k")
			passwordModeOn(*ch);
		else if (option == "-k")
			passwordModeOff(*ch);
		else if (option == "+l")
			userLimitModeOn(*ch);
		else if (option == "-l")
			userLimitModeOff(*ch);
		else
		{
			std::string reply = ":ircap 300 ERROR: Option was not found!\r\n";
			send(_fd, reply.c_str(), reply.length(), 0);
			return ;
		}
	}
	else
	{
		std::string reply = ":ircap 482 " + this->nickname + " #" + chan + " :You're not channel operator\r\n";
		send(_fd, reply.c_str(), reply.length(), 0);
	}
}
