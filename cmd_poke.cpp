#include "Server.hpp"

// Channel side

bool	Channel::pokeSpawn(void) {
	if (this->_pokespawned)
		return (false);

	std::map<std::string, int> pokimap;
	pokimap["Pikachu"] = 2;
	pokimap["Bulbasaur"] = 3;
	pokimap["Charmander"] = 3;
	pokimap["Squirtle"] = 3;
	pokimap["Ditto"] = 7;
	pokimap["Mew"] = 42;
	pokimap["Snorlax"] = 5;
	pokimap["Cubone"] = 4;
	pokimap["Abra"] = 4;
	pokimap["Growlithe"] = 5;

	std::pair<std::string, int> chosen;
	std::vector<std::pair<std::string, int> > vec(pokimap.begin(), pokimap.end());

	std::srand(std::time(0));

    chosen = vec[std::rand() % pokimap.size()];
    this->_pokename = chosen.first;
	this->_pokechance = chosen.second;
	this->_pokespawned = true;

	return (true);
}

bool		Channel::pokeIsSpawned(void) { return (this->_pokespawned); }

std::string	Channel::pokeName(void) const { return (this->_pokename); }

bool	Channel::pokeCatch(void) {
	std::srand(std::time(0));

    if (std::rand() % this->_pokechance == 0) {
		//send you did it to channel
		this->_pokespawned = false;
		return (true);
	}
	//u cazz to channel
	return (false);
}

// Command execution

static void	ft_sendchannel(Channel &ch, std::string &msg) {
	
	for (std::set<int>::const_iterator each = ch._clients.begin(); each != ch._clients.end(); ++each) {
		send((*each), msg.c_str(), msg.length(), 0);
	}
}

std::string	ft_setpokedex(t_server &srv, Client &they, Channel &ch) {
	int n;

	std::string msg = ":pokecap! PRIVMSG #" + ch.getName() + " :This is " + they.getNick() + " pokedex:\n";//\r\n;
	for (int i = 0; i < 10; ++i) {
		n = they.getPokedex().count(srv.pokemons[i]);
		if (n)
			msg += n + " " + srv.pokemons[i] + "\n";
	}
	msg += "\r\n";

	return (msg);
}

void						Client::poke(t_server &srv) {
	Channel 			*ch;
	std::string			tmp, ch_name, what, who;
	std::istringstream	buf(srv.command);

	std::getline(buf, tmp, ' ');
	std::getline(buf, ch_name, ' ');
	std::getline(buf, what, ' ');
	std::getline(buf, who, ' ');

	if (ch_name[0] == '#' || ch_name[0] == '@')
		ch_name.erase(ch_name.begin());
	else {
		tmp = ":ircap 403 " + this->nickname + " " + ch_name + " :Not '#' in channel name\r\n";
		send(this->_fd, tmp.c_str(), tmp.length(), 0);
		return ;
	}

	if (!srv.channels.count(ch_name)) {
		tmp = ":ircap 403 " + this->nickname + " #" + ch_name + " :No such channel\r\n";
		send(this->_fd, tmp.c_str(), tmp.length(), 0);
		return ;
	}

	ch = &(srv.channels.find(ch_name)->second);

	if (!ch->_clients.count(this->_fd)) {
		tmp = ":ircap 442 " + this->nickname + " #" + ch_name + " :You're not on that channel\r\n";
		send(this->_fd, tmp.c_str(), tmp.length(), 0);
	}
	else if (!what.compare("spawn")) {
		if (ch->pokeSpawn() == false) {
			tmp = ":pokecap! PRIVMSG #" + ch->getName() + " :A " + ch->pokeName() + "has appeared! Try to catch it!\r\n";
			ft_sendchannel(*ch, tmp);
		}
		else {
			tmp = ":ircap 442 " + this->nickname + " #" + ch_name + " :A pokemon is already spawned!\r\n";
			send(this->_fd, tmp.c_str(), tmp.length(), 0);
		}
	}
	else if (!what.compare("catch")) {
		if (ch->pokeCatch())
			this->addPoke(ch->pokeName());
	}
	else if (!what.compare("pokedex")) {
		tmp = ft_setpokedex(srv, *this, *ch);

		if (!who.compare("all"))
			ft_sendchannel(*ch, tmp);
		else
			send(this->_fd, tmp.c_str(), tmp.length(), 0);
	}

}

std::multiset<std::string>		Client::getPokedex(void) const {
	return (this->_pokedex);
}

void						Client::addPoke(std::string name) { this->_pokedex.insert(name); }

	// pokimap.push_back("Pikachu");
	// pokimap.push_back("Bulbasaur");
	// pokimap.push_back("Charmander");
	// pokimap.push_back("Squirtle");
	// pokimap.push_back("Ditto");
	// pokimap.push_back("Mew");
	// pokimap.push_back("Snorlax");
	// pokimap.push_back("Cubone");
	// pokimap.push_back("Abra");
	// pokimap.push_back("Growlithe");
