#include "Server.hpp"

//* ERR_NOTONCHANNEL (442) => You're not on that channel
//* ERR_CHANOPRIVSNEEDED (482) => You're not channel operator
//* ERR_NOSUCHCHANNEL (403) => No such channel
//* ERR_USERONCHANNEL (443) => client is already on channel
// RPL_INVITING (341) //! invite sent successfully
//? :dan-!d@localhost INVITE Wiz #test => msg when the invite is successful for the client
//? INVITE Wiz #foo_bar => the command to invite a client to a channel

void	Client::invite(t_server &srv)
{
	std::istringstream iss(srv.buffer);
	std::string cmd, cmd_nickname, chan;
	std::getline(iss, cmd, ' ');
	std::getline(iss, cmd_nickname, ' ');
	std::getline(iss, chan, '\n'); //TODO: if another client command might end with another command the just \n
	if (chan[0] == '#')
		chan.erase(chan.begin());

	if (cmd_nickname.empty() || chan.empty())
	{
		std::string reply = "\033[31mERROR :Command is not formated correctly!\033[0m\r\n";
		send(_fd, reply.c_str(), reply.length(), 0);
		return ;
	}
	if (!srv.channels[chan].isInChannel(srv.nicknames[this->nickname]))
	{
		std::string reply = "\033[31mERR_NOTONCHANNEL (442) :you are not even in the channel\033[0m\r\n";
		send(_fd, reply.c_str(), reply.length(), 0);
		return ;
	}
	if (this->isOpInChannel(chan, srv) == false)
	{
		std::string reply = "ERR_CHANOPRIVSNEEDED (482) :You're not channel operator\r\n";
		send(_fd, reply.c_str(), reply.length(), 0);
		return ;
	}
	if (srv.channels.find(chan) == srv.channels.end())
	{
		std::string reply = "ERR_NOSUCHCHANNEL (403) :No such channel\r\n";
		send(_fd, reply.c_str(), reply.length(), 0);
		return ;
	}
	if (srv.channels[chan].isInChannel(srv.nicknames[cmd_nickname]))
	{
		std::string reply = "ERR_USERONCHANNEL (443) :the client is already in the channel\r\n";
		send(_fd, reply.c_str(), reply.length(), 0);
		return ;
	}

	//TODO: invite the client to the channel
	srv.channels[chan]._invited.insert(cmd_nickname);
	
	//TODO: send the replay to the operator
	std::string reply = "RPL_INVITING (341) :invite sent successfully\r\n";
	send(_fd, reply.c_str(), reply.length(), 0);

	//TODO: send the success msg to the client
	std::string msg = ":" + this->nickname + "!" + this->username + "@localhost INVITE " + cmd_nickname + " :" + chan + "\r\n";
	send(srv.nicknames[cmd_nickname], msg.c_str(), msg.length(), 0);

}