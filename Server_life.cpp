#include "Server.hpp"
#include "Client.hpp"
#include <sstream>

void	wait_events(t_server &srv, int &ev_nums) {
	ev_nums = epoll_wait(srv.poll_fd, srv.ev_lst, MAX_EVENTS, -1);
	if (ev_nums == -1)
		perror("Error in epoll() or CTRL+C recived:");
}

int console_event(t_server &srv) {
	std::cin >> srv.buffer;
	if (!strcmp(srv.buffer, "quit") || !strcmp(srv.buffer, "exit")) {
		server_life = 0;
		return (1);
	}
	else if (!strcmp(srv.buffer, "clients"))
		printClients(srv);
	else if (!strcmp(srv.buffer, "channels"))
		printChannels(srv);
	return (0);
}

void new_client_event(t_server &srv) {
	int result;
	static int index = 0;
	
	srv.client_fd = accept(srv.socket, (sockaddr *)&srv.client_addr, &srv.client_addr_len);
	if (srv.client_fd < 0) {
		std::cerr << "Failed to accept the client" << std::endl;
		close(srv.socket);
		return ;
	}
	srv.ev_def.events = EPOLLIN;
	srv.ev_def.data.fd = srv.client_fd;
	
	//! ---------------------- new client in  map -------------------------------- */
	srv.client_map[srv.client_fd] = new Client(srv.client_fd, index++);
	//aggiungi a mappa

	result = epoll_ctl(srv.poll_fd, EPOLL_CTL_ADD, srv.client_fd, &srv.ev_def);
	if (result) {
		perror("Error adding client to epoll");
		return ;
	}
}

bool process_passwd(t_server &srv, std::map<int, Client*>::iterator &it, std::string &arg) {
	std::string konversation = ":";
	konversation += srv.passwd;
	if (arg != srv.passwd && arg != konversation)
	{
		std::string wrong = "Err_num(464) Wrong password\r\n";
		send(srv.client_fd, wrong.c_str(), wrong.length(), 0);
		srv.client_map.erase(it);
		delete it->second;
		close(srv.client_fd);
		return (false);
	}
	else
	{
		std::string reply = "Welcome to the Internet Relay Network\r\n";
		send(srv.client_fd, reply.c_str(), reply.length(), 0);
		srv.client_map[srv.client_fd]->setAuthenticate(true);
	}
	return (true);
}

/* my baby */

void	new_cmd_event(t_server &srv, int i)
{
	srv.check = true;
	//! ------------------- iterate and try to find the client ------------------- */
	std::map<int, Client*>::iterator it = srv.client_map.find(srv.ev_lst[i].data.fd);
	srv.client_fd = it->first;

	bzero(srv.buffer, sizeof(srv.buffer));
	srv.bytes_read = recv(srv.client_fd, srv.buffer, sizeof(srv.buffer), 0);

	if (!srv.bytes_read)
		ft_client_quit(srv, i);
	else if (srv.bytes_read < 0)
		std::cerr << "Error while receving the message" << std::endl;
	else
	{
		std::size_t lngt;
		std::string tmp, halfbuf;
		bool		passwd_ok;
		
		passwd_ok = true;
		tmp = srv.client_map[srv.client_fd]->getHalfbuf() + srv.buffer;
		
		if (tmp[tmp.length() - 1] != '\n') {
			halfbuf = tmp.substr(tmp.rfind('\n') + 1, tmp.length());
			tmp.erase(tmp.rfind('\n') + 1, tmp.length());
			srv.client_map[srv.client_fd]->setHalfbuf(halfbuf);
		}

		std::istringstream iss_buf(tmp);
		std::getline(iss_buf, srv.command, '\n');
		lngt = srv.command.length();
		while (lngt && passwd_ok && srv.check) {
			if (srv.command[lngt - 1] == '\r')
				srv.command.erase(lngt - 1 ,1);

			std::istringstream iss(srv.command);
			std::string cmd, arg;
			std::getline(iss, cmd, ' ');
			std::getline(iss, arg);

			std::cout << "\nclient send: " << srv.command << std::endl;

			if (cmd == "QUIT")
				ft_client_quit(srv, i);
			else if (srv.client_map[srv.client_fd]->getAuthenticate() == true)
			{
				if (cmd == "NICK")
					ft_set_nick(srv, i);
				else if (!srv.client_map[srv.client_fd]->getNick().empty())
					srv.client_map[srv.client_fd]->handleCmd(srv.command, srv);
			}
			else if (cmd == "PASS")
				passwd_ok = process_passwd(srv, it, arg);

			std::getline(iss_buf, srv.command, '\n');
			lngt = srv.command.length();
		}
	}
}

void	ft_server_life(t_server &srv) {
	int	ev_nums;
	
	while (server_life)
	{
		wait_events(srv, ev_nums);

		if (ev_nums == -1)
			continue ;

		for (int i = 0; i < ev_nums; i++)
		{
			if (srv.ev_lst[i].data.fd == STDIN_FILENO) {
				if (console_event(srv))
					break;
			}
			else if (srv.ev_lst[i].data.fd == sock_fd)
				new_client_event(srv);
			else
				new_cmd_event(srv, i);
		}
	}
}

