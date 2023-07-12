#include "Server.hpp"
#include "Client.hpp"
#include <sstream>

int	server_exit(t_server &srv, int exit_code) {
	close(srv.client_fd);
	close(srv.socket);
	close(srv.poll_fd);
	/* loop for al list */

	exit(exit_code);
}

void	wait_events(t_server &srv, int &ev_nums) {
	ev_nums = epoll_wait(srv.poll_fd, srv.ev_lst, MAX_EVENTS, -1);
	if (ev_nums == -1) {
		perror("Error in epoll()");
		// server_exit(srv, 1);
	}
	// std::cout << "num events: " << ev_nums << std::endl;
}

void console_event(t_server &srv) {
	std::cin >> srv.buffer;
	if (!strcmp(srv.buffer, "quit") || !strcmp(srv.buffer, "exit"))
		server_exit(srv, 1);
	else if (!strcmp(srv.buffer, "clients"))
		printClients(srv);
	else if (!strcmp(srv.buffer, "channels"))
		printChannels(srv);
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
	//new client;
	//srv.client_map.new()  <--> map <int => clinet_fd, client => new client>
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
	// std::cout << "arg: " << arg << std::endl;
	// std::cout << "srv.passwd: " << srv.passwd << std::endl;
	// std::cout << "konversation: " << konversation << std::endl;
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

	if (!srv.bytes_read) {
		// std::cerr << "Connection closed by the client" << std::endl;
		ft_client_quit(srv, i);
		// epoll_ctl(srv.poll_fd, EPOLL_CTL_DEL, srv.client_fd, &srv.ev_lst[i]);
		// // close(srv.client_fd);/* remove epoll_ctl */
		// //! ------------------- remove from map ------------------- */
		
		// //send disconect to all channels and remove from all channels
		
		// srv.client_map.erase(it);
		// delete it->second;
	}
	else if (srv.bytes_read < 0)
		std::cerr << "Error while receving the message" << std::endl;
	else
	{
		std::cout << "buffer: " << srv.buffer << std::endl;
		std::istringstream iss_buf(srv.buffer);
		std::string tmp;
		std::size_t lngt;
		bool	passwd_ok;

		passwd_ok = true;
		std::getline(iss_buf, srv.command, '\n');
		lngt = srv.command.length();
		while (lngt && passwd_ok && srv.check) {
			std::cout << "command" << srv.command << std::endl;
			if (srv.command[lngt - 1] == '\r')
				srv.command.erase(lngt - 1 ,1);

			std::istringstream iss(srv.command);

			std::string cmd, arg;
			std::getline(iss, cmd, ' ');
			std::getline(iss, arg);
			if (cmd == "QUIT")
				ft_client_quit(srv, i);
			else if (srv.client_map[srv.client_fd]->getAuthenticate() == true)
			{
				std::cout << "client send: " << srv.command << std::endl;
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

		for (int i = 0; i < ev_nums; i++)
		{
			if (srv.ev_lst[i].data.fd == STDIN_FILENO)
				console_event(srv);
			else if (srv.ev_lst[i].data.fd == sock_fd)
				new_client_event(srv);
			else
				new_cmd_event(srv, i);
		}
	}
}

