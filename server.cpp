/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ggiannit <ggiannit@student.42firenze.it    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 15:31:16 by ggiannit          #+#    #+#             */
/*   Updated: 2023/07/23 18:39:59 by ggiannit         ###   ########.fr       */
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

int		sock_fd = 0;
int		server_life = 1;
bool	nick_used = false;

void	server_exit(t_server &srv) {
	close(srv.socket);
	close(srv.poll_fd);

	/* loop for al list */
	std::map<int, Client *>::iterator it = srv.client_map.begin();

	while (it != srv.client_map.end()) {
		close(it->first);
		delete it->second;
		++it;
	}
}

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

	server_exit(srv); 
	return (0);
}
