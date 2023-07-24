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
	Channel *ch;
	std::istringstream iss(srv.command);
	std::string cmd, cmd_nickname, chan;
	std::getline(iss, cmd, ' ');
	std::getline(iss, cmd_nickname, ' ');
	std::getline(iss, chan);


	if (chan[0] == '#' || chan[0] == '@')
		chan.erase(chan.begin());
	while (chan[chan.size() - 1] == '\n' || chan[chan.size() - 1] == '\r' || chan[chan.size() - 1] == ' ')
		chan.erase(chan.end() - 1);

	if (!srv.channels.count(chan)) {
		cmd = ":ircap 403 " + this->nickname + " #" + chan + " :No such channel\r\n"; 
		send(this->_fd, cmd.c_str(), cmd.length(), 0);
		return ;
	}
	
	ch = &(srv.channels.find(chan)->second);

	if (!ch->_clients.count(this->_fd)) {
		cmd = ":ircap 442 " + this->nickname + " #" + chan + " :You're not on that channel\r\n";
		send(this->_fd, cmd.c_str(), cmd.length(), 0);
	}
	else if (!ch->_operators.count(this->nickname)) {
		cmd = ":ircap 482 " + this->nickname + " #" + chan + " :You're not channel operator\r\n";
		send(this->_fd, cmd.c_str(), cmd.length(), 0);
	}
	else if (ch->_clients.count(srv.nicknames[cmd_nickname])) {
		cmd = ":ircap 443 " + this->nickname + " #" + chan + " :The client is already in the channel\r\n";
		send(_fd, cmd.c_str(), cmd.length(), 0);
	}
	else {
		// invite the client to the channel
		ch->_invited.insert(cmd_nickname);
		
		// send the replay to the operator
		std::string reply = ":ircap 341 " + this->nickname + " #" + chan + " :invite sent successfully\r\n";
		send(_fd, reply.c_str(), reply.length(), 0);

		// send the success msg to the client
		std::string msg = ":" + this->nickname + "!" + this->username + "@localhost INVITE " + cmd_nickname + " :" + chan + "\r\n";
		send(srv.nicknames[cmd_nickname], msg.c_str(), msg.length(), 0);

	}

}
