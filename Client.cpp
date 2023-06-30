#include "Client.hpp"

Client::Client(int const &fd) : _fd(fd) , _authenticate(false) {}

void	Client::set_authenticate(bool auth) { _authenticate = auth; }

bool	Client::get_authenticate() const { return _authenticate; }

Client::~Client() {}