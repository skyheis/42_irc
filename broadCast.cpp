#include "Client.hpp"


void	Client::inviteOnlyModeOn(Channel &chan)
{
	chan.setMode(MD_I, true);
	std::string msg = ":" + this->nickname + " MODE #" + chan.getName() + " +i\r\n";
	for (std::set<int>::const_iterator it = chan._clients.begin(); it != chan._clients.end(); ++it)
		send((*it), msg.c_str(), msg.length(), 0);
}

void	Client::inviteOnlyModeOff(Channel &chan)
{
	chan.setMode(MD_I, false);
	std::string msg = ":" + this->nickname + " MODE #" + chan.getName() + " -i\r\n";
	for (std::set<int>::const_iterator it = chan._clients.begin(); it != chan._clients.end(); ++it)
		send((*it), msg.c_str(), msg.length(), 0);
}

void	Client::topicModeOn(Channel &chan)
{
	chan.setMode(MD_T, true);
	std::string msg = ":" + this->nickname + " MODE #" + chan.getName() + " +t\r\n";
	for (std::set<int>::const_iterator it = chan._clients.begin(); it != chan._clients.end(); ++it)
		send((*it), msg.c_str(), msg.length(), 0);
}

void	Client::topicModeOff(Channel &chan)
{
	chan.setMode(MD_T, false);
	std::string msg = ":" + this->nickname + " MODE #" + chan.getName() + " -t\r\n";
	for (std::set<int>::const_iterator it = chan._clients.begin(); it != chan._clients.end(); ++it)
		send((*it), msg.c_str(), msg.length(), 0);
}

void	Client::passwordModeOn(Channel &chan)
{
	chan.setMode(MD_K, true);
	std::string msg = ":" + this->nickname + " MODE #" + chan.getName() + " +k\r\n";
	for (std::set<int>::const_iterator it = chan._clients.begin(); it != chan._clients.end(); ++it)
		send((*it), msg.c_str(), msg.length(), 0);
}

void	Client::passwordModeOff(Channel &chan)
{
	chan.setMode(MD_K, false);
	std::string msg = ":" + this->nickname + " MODE #" + chan.getName() + " -k\r\n";
	for (std::set<int>::const_iterator it = chan._clients.begin(); it != chan._clients.end(); ++it)
		send((*it), msg.c_str(), msg.length(), 0);
}

void	Client::userLimitModeOn(Channel &chan)
{
	chan.setMode(MD_L, true);
	std::string msg = ":" + this->nickname + " MODE #" + chan.getName() + " +l\r\n";
	for (std::set<int>::const_iterator it = chan._clients.begin(); it != chan._clients.end(); ++it)
		send((*it), msg.c_str(), msg.length(), 0);
}

void	Client::userLimitModeOff(Channel &chan)
{
	chan.setMode(MD_L, false);
	std::string msg = ":" + this->nickname + " MODE #" + chan.getName() + " -l\r\n";
	for (std::set<int>::const_iterator it = chan._clients.begin(); it != chan._clients.end(); ++it)
		send((*it), msg.c_str(), msg.length(), 0);
}
