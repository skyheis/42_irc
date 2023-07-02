#include "Client.hpp"

void	Client::joinChannel(t_server &srv) {
	if (srv.buffer[5] != '#' && srv.buffer[5] != '&')
		return ; //error but i guess is not possible

	std::istringstream buf(srv.buffer);
	std::string tmp, ch_names, keys;
	std::getline(buf, tmp, ' ');
	std::getline(buf, ch_names, ' ');
	std::getline(buf, keys, ' ');
	
	while (ch_names.find(',') != std::string::npos)
	{
		// tmp = ch_names;
		// tmp.erase(tmp.begin());
		// tmp.erase(tmp.find(','), tmp.length());
		tmp = ch_names.substr(0, ch_names.find(','));
		ch_names.erase(0, tmp.length() - 1);
		tmp.erase(tmp.begin());
		

		if (srv.channels.find(tmp) != srv.channels.end()) {
			std::map<std::string, Channel>::iterator it = srv.channels.find(tmp);

			if (it->second.getMode(MD_K) != )
		}
	}
		
}

