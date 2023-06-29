/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiannit <ggiannit@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 15:31:16 by ggiannit          #+#    #+#             */
/*   Updated: 2023/06/29 19:23:14 by ggiannit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "unistd.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include <sys/epoll.h>
#include <cstring>
#include <csignal>
#include <cstdio>
#include <cstdlib>

#define MAX_BUF 512


int sock_fd = 0;
int	server_life = 1;

void	signalHandler(int signum) {
	std::cout << "Interrupt signal (" << signum << ") received." << std::endl;
	server_life = 0;
	close(sock_fd);
	exit(1);
}

int	main() {
	in_port_t port = 6642;
	
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_fd < 0) {
		std::cerr << "Error creating socket" << std::endl;
		return 1;
	}
	std::cout << "Socket created" << std::endl;
	
	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET; // IPv4
	server_addr.sin_addr.s_addr = INADDR_ANY; // Any IP address
	server_addr.sin_port = htons(port); // Port

	int bind_status = bind(sock_fd, (sockaddr *)&server_addr, sizeof(server_addr));
	if (bind_status) {
		std::cerr << "Failed to bind socket to address" << std::endl;
		close(sock_fd);
		return 1;
	}
	std::cout << "Socket bound to address" << std::endl;
	
	int listen_status = listen(sock_fd, 128);
	if (listen_status){
		std::cerr << "Failed to listen for connections" << std::endl;
		close(sock_fd);
		return 1;
	}
	std::cout << "Socket ready to listen for connections" << std::endl;	


	signal(SIGINT, signalHandler);


	int epfd = epoll_create1(0);
	struct epoll_event defev;
	defev.events = EPOLLIN;
	defev.data.fd = STDIN_FILENO;
	
	int ctl_status = epoll_ctl(epfd, EPOLL_CTL_ADD, STDIN_FILENO, &defev);
	if (ctl_status){
		std::cerr << "epoll_ctl() filed :c" << std::endl;
		close(sock_fd);
		return 1;
	}
	defev.data.fd = sock_fd;
	ctl_status = epoll_ctl(epfd, EPOLL_CTL_ADD, sock_fd, &defev);
	if (ctl_status){
		std::cerr << "epoll_ctl() filed :c" << std::endl;
		close(sock_fd);
		return 1;
	}

	struct epoll_event event_list[1024];
	int	num_events;
	sockaddr_in client_addr;
	socklen_t	client_addr_len = sizeof(client_addr);
	int client_fd;
	char	buffer[MAX_BUF];
	ssize_t bytes_read;



	std::cout << "Entering server life\n" << std::endl;
	while (server_life)
	{
		num_events = epoll_wait(epfd, event_list, 1024, -1);
		if (num_events == -1) {
			std::cerr << "epoll_ctl() filed :c" << std::endl;
			close(sock_fd);
			return 1;
		}
		
		for (int i = 0; i < num_events; i++){
			if (event_list[i].data.fd == STDIN_FILENO) {
				std::cin >> buffer;
				if (!strcmp(buffer, "quit") || !strcmp(buffer, "exit"))
                {
                    close(sock_fd);
                    return (0);
                }
			}
			else if (event_list[i].data.fd == sock_fd) {
				client_fd = accept(sock_fd, (sockaddr *)&client_addr, &client_addr_len);
				if (client_fd < 0) {
					std::cerr << "Failed to accept the client" << std::endl;
					close(sock_fd);
					continue;
				}

				defev.events = EPOLLIN;
                defev.data.fd = client_fd;
                
				ctl_status = epoll_ctl(epfd, EPOLL_CTL_ADD, client_fd, &defev);
     	      	if (ctl_status){
					std::cerr << "epoll_ctl() filed :c" << std::endl;
					close(sock_fd);
					return 1;
				}
			}
			else {
				client_fd = event_list[i].data.fd;
				bytes_read = recv(client_fd, buffer, sizeof(buffer), 0);
				if (!bytes_read) {
					std::cerr << "Connection closed by the client" << std::endl;
					epoll_ctl(epfd, EPOLL_CTL_DEL, client_fd, &event_list[i]);
					// close(client_fd);/* remove epoll_ctl */
				}
				else if (bytes_read < 0)
					std::cerr << "Error while receving the message" << std::endl;
				else {
					buffer[bytes_read] = '\0';
					std::cout << "client"<< i << ":\t" << buffer << std::endl;
				}
				
			}
		}
	}


	close(sock_fd);
	return (0);
}

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
