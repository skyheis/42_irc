#include "Client.hpp"


void	Client::inviteOnlyModeOn(t_server &srv, std::string chan)
{
	srv.channels[chan].setMode(MD_I, true);
	std::string msg = "\033[32mInvite only mode has been enabled with success!\033[0m\r\n";
	for (std::set<int>::const_iterator it = srv.channels[chan]._clients.begin(); it != srv.channels[chan]._clients.end(); ++it)
		send((*it), msg.c_str(), msg.length(), 0);
}

void	Client::inviteOnlyModeOff(t_server &srv, std::string chan)
{
	srv.channels[chan].setMode(MD_I, false);
	std::string msg = "\033[32mInvite only mode has been disabled with success!\033[0m\r\n";
	for (std::set<int>::const_iterator it = srv.channels[chan]._clients.begin(); it != srv.channels[chan]._clients.end(); ++it)
		send((*it), msg.c_str(), msg.length(), 0);
}

void	Client::topicModeOn(t_server &srv, std::string chan)
{
	srv.channels[chan].setMode(MD_T, true);
	std::string msg = "\033[32mTopic mode has been enabled with success!\033[0m\r\n";
	for (std::set<int>::const_iterator it = srv.channels[chan]._clients.begin(); it != srv.channels[chan]._clients.end(); ++it)
		send((*it), msg.c_str(), msg.length(), 0);
}

void	Client::topicModeOff(t_server &srv, std::string chan)
{
	srv.channels[chan].setMode(MD_T, false);
	std::string msg = "\033[32mTopic mode has been disabled with success!\033[0m\r\n";
	for (std::set<int>::const_iterator it = srv.channels[chan]._clients.begin(); it != srv.channels[chan]._clients.end(); ++it)
		send((*it), msg.c_str(), msg.length(), 0);
}

void	Client::passwordModeOn(t_server &srv, std::string chan)
{
	srv.channels[chan].setMode(MD_K, true);
	std::string msg = "\033[32mChannel key mode has been enabled with success!\033[0m\r\n";
	for (std::set<int>::const_iterator it = srv.channels[chan]._clients.begin(); it != srv.channels[chan]._clients.end(); ++it)
		send((*it), msg.c_str(), msg.length(), 0);
}

void	Client::passwordModeOff(t_server &srv, std::string chan)
{
	srv.channels[chan].setMode(MD_K, false);
	std::string msg = "\033[32mChannel key mode has been disabled with success!\033[0m\r\n";
	for (std::set<int>::const_iterator it = srv.channels[chan]._clients.begin(); it != srv.channels[chan]._clients.end(); ++it)
		send((*it), msg.c_str(), msg.length(), 0);
}

void	Client::userLimitModeOn(t_server &srv, std::string chan)
{
	srv.channels[chan].setMode(MD_L, true);
	std::string msg = "\033[32mUser limit mode has been enabled with success!\033[0m\r\n";
	for (std::set<int>::const_iterator it = srv.channels[chan]._clients.begin(); it != srv.channels[chan]._clients.end(); ++it)
		send((*it), msg.c_str(), msg.length(), 0);
}

void	Client::userLimitModeOff(t_server &srv, std::string chan)
{
	srv.channels[chan].setMode(MD_L, false);
	std::string msg = "\033[32mUser limit mode has been disabled with success!\033[0m\r\n";
	for (std::set<int>::const_iterator it = srv.channels[chan]._clients.begin(); it != srv.channels[chan]._clients.end(); ++it)
		send((*it), msg.c_str(), msg.length(), 0);
}