/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiannit <ggiannit@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 15:31:16 by ggiannit          #+#    #+#             */
/*   Updated: 2023/06/30 18:32:51 by ggiannit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "unistd.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include <sys/epoll.h>
#include <fcntl.h>
#include <cstring>
#include <csignal>
#include <cstdio>
#include <cstdlib>

#include "Server.hpp"

int sock_fd = 0;
int	server_life = 1;

int	main(int ac, char **av) {
	t_server	srv;

	if (ac != 3) {
		std::cerr << "Please enter provide the folling argument:\n\t\
		Port [1024-65535]\n\tPassword [3+ chars]\n\n\
		ex: ./server 6642 foo" << std::endl;	
		return (1);
	}
	
	init_server(av[1], av[2], srv);

	std::cout << "Starting server life...\n" << std::endl;
	ft_server_life(srv);
	
	close(sock_fd);
	return (0);
}

// void	signalHandler(int signum) {
// 	std::cout << "Interrupt signal (" << signum << ") received." << std::endl;
// 	server_life = 0;
// 	close(sock_fd);
// 	exit(1);
// }


/* from server test */
/* 	while(server_life) {
		std::cout << std::endl;

		sockaddr_in client_addr;
		socklen_t	client_addr_len = sizeof(client_addr);
		
		int client_fd = accept(sock_fd, (sockaddr *)&client_addr, &client_addr_len);
		if (client_fd < 0) {
			std::cerr << "Failed to accept the client" << std::endl;
			close(client_fd);
			continue;
		}

		char buffer[1024];
		ssize_t bytes_read = recv(client_fd, buffer, sizeof(buffer), 0);
		
		if (!bytes_read)
			std::cerr << "Connection closed by the client" << std::endl;
		else if (bytes_read < 0)
			std::cerr << "Error while receving the message" << std::endl;
		else {
			buffer[bytes_read] = '\0';
			std::cout << "New message from client: " << buffer << std::endl;
		}


		const char *message = "Hello, from server!";
		ssize_t bytes_sent = send(client_fd, message, strlen(message), 0);
		
		if (bytes_sent < 0)
			std::cerr << "Error while sending the message" << std::endl;
		else
			std::cerr << "Message successfully sent!" << std::endl;

		close(client_fd);
	} */
