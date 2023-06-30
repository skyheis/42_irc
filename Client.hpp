#pragma once

#include <iostream>
#include <string>

class Client
{
	private:
		int const	_fd;
		bool 		_authenticate;
		std::string nickname;
		std::string	passwd;
		Client();
	public:
		Client(int const &fd);

		void	set_authenticate(bool auth);
		bool	get_authenticate() const ;
		// std::string	_half_buffer;
		// vector<>;

		~Client();
};