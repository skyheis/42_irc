# ft_irc for 42

## Description

This project is about creating an IRC server in c++98.
The server must be able to handle multiple clients at the same time. You should choose a specific client that must works with your server, like it works with the real IRC servers. We choose `hexchat` for this project.
It must be able to handle the following commands:
- NICK
- JOIN
- PART
- KICK
- MODE (only channel modes)
- PRIVMSG
- INVITE
- QUIT
- TOPIC
The rest of the commands are optional.

## Usage
To compile the server, run `make` in the root of the repository. This will create a binary called `ircserv`.
To run the server, run `./ircserv <port> <password>`. The server will listen on the specified port.
To connect to the server, run `nc <ip> <port>`.

## Bonus
The bonus part is to implement the file transfer protocol DCC and a bot. For file transfer we choose to implement the DCC SEND command. For the bot we choose to implement a pokemon minigame where you can catch pokemons in the channel. Here the list of pokemon commands:
- `/poke #<channel-name> spawn` - spawn a random pokemon in the channel.
- `/poke #<channel-name> catch` - try to catch a pokemon, each pokemon has a different catch rate, after 4 tries the pokemon will run away.
- `/poke #<channel-name> pokedex` - list all the pokemons only for you.
- `/poke #<channel-name> pokedex all` - list all the pokemons for everyone.
About the file transfer, you can send a file to a user using the integrated `send` command of private chat in hexchat. You can also send a file using nc with the following command:
```bash
PRIVMSG <client> :DCC SEND <filename> <ip>  <port>
```

## Authors
- [**ggiannit**](https://github.com/skyheis)
- [**naal-jen**](https://github.com/Nazar963)
