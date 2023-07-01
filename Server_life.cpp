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
		server_exit(srv, 1);
	}
	// std::cout << "num events: " << ev_nums << std::endl;
}

void console_event(t_server &srv) {
	std::cin >> srv.buffer;
	if (!strcmp(srv.buffer, "quit") || !strcmp(srv.buffer, "exit"))
		server_exit(srv, 1);
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

void	new_cmd_event(t_server &srv, int i) {
	//Client &sdsadas = map[srv.ev_lst[i].data.fd];
	//! ------------------- iterate and try to find the client ------------------- */
	std::map<int, Client*>::iterator it = srv.client_map.find(srv.ev_lst[i].data.fd);

	// srv.client_fd = srv.ev_lst[i].data.fd;
	int client_fd = it->first;
	// Client* client = it->second;
	srv.bytes_read = recv(client_fd, srv.buffer, sizeof(srv.buffer), 0);
	if (!srv.bytes_read) {
		std::cerr << "Connection closed by the client" << std::endl;
		epoll_ctl(srv.poll_fd, EPOLL_CTL_DEL, client_fd, &srv.ev_lst[i]);
		// close(srv.client_fd);/* remove epoll_ctl */
		//! ------------------- remove from map ------------------- */
		srv.client_map.erase(it);
		delete it->second;
	}
	else if (srv.bytes_read < 0)
		std::cerr << "Error while receving the message" << std::endl;
	else
	{
		//! ---------------- test to show there is no \r in the buffer --------------- */
		// char *ptr = std::strchr(srv.buffer, '\r');
		// if (ptr)
		// 	std::cout << "ptr: " << ptr << std::endl;
		// else
		// 	std::cout << "there is no character found" << std::endl;
		if (srv.buffer[srv.bytes_read - 1] == '\n')
		{
			srv.buffer[srv.bytes_read - 1] = '\0';

			std::istringstream iss(srv.buffer);

			std::string command, arg;
			std::getline(iss, command, ' ');
			std::getline(iss, arg, ' ');
			if (srv.client_map[client_fd]->get_authenticate() == false)
			{
				if (command == "PASS")
				{
					if (arg != srv.passwd)
					{
						std::string wrong = "Err_num(464) Wrong password\r\n";
						send(client_fd, wrong.c_str(), wrong.length(), 0);
						srv.client_map.erase(it);
						delete it->second;
						close(client_fd);
					}
					else
					{
						std::string reply = "Welcome to the Internet Relay Network\r\n";
						send(client_fd, reply.c_str(), reply.length(), 0);
						srv.client_map[client_fd]->set_authenticate(true);
					}
				}
				else
				{
					std::string reply = "ERROR :Password required\r\n";
					send(client_fd, reply.c_str(), reply.length(), 0);
					close(client_fd);
					srv.client_map.erase(it);
					delete it->second;
				}
			}
			else
				srv.client_map[client_fd]->handleCmd(srv.buffer, srv);
		}
		else
			std::cerr << "Messege recieved is not formated correctly" << std::endl;
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

