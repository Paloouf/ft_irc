#include "../include/Server.hpp"

std::string	Server::convertIP(const void *address)
{
	const unsigned char *caddress = (const unsigned char *)address;

	for (int i = 0; caddress[i] != '\0'; i++)
		std::cout << caddress[i] << " " << i << std::endl;
	return ("truc");
}

void		Server::checkInput()
{
	for (int i = 0; _port[i] != 0; i++)
		if (!std::isdigit(_port[i]))
			throw Server::portNonDigit();
	if (std::atol(_port.c_str()) > 64738 || _port.size() > 5)
		throw Server::portTooHigh();
}

Server::~Server(){
	std::cout << "Server dead\n";}

Server::Server(std::string port, std::string password): _port(port), _password(password)
{
	std::cout << "Server active" << std::endl;
	try {checkInput();}
	catch (Server::portNonDigit& error)
		{std::cerr << error.what() << std::endl;}
	catch (Server::portTooHigh& error)
		{std::cerr << error.what() << std::endl;}
	listening();
}

std::string	Server::getPassword(){
	return (_password);}

std::string Server::getPort(){
	return (_port);}

void	Server::listening(){
	struct sockaddr_in address;
	struct in_addr addr;
	addr.s_addr = INADDR_ANY;

	_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	address.sin_family = AF_INET;
	address.sin_addr = addr;
	address.sin_port = htons(std::atoi(_port.c_str()));

	bind(_sockfd, (struct sockaddr*)&address, sizeof(address));
	listen(_sockfd, 8);
	std::cout << "Waiting for connection...\n";
	createFd();
	while (1)
		waitInput();
}

void	Server::createFd(){
	this->_clientsFd = new struct pollfd[9];

	this->_clientsFd[0].fd = this->_sockfd;
	this->_clientsFd[0].events = POLLIN;

	for (unsigned long i = 0; i < this->_clients.size(); i++)
	{
		this->_clientsFd[i + 1].fd = 0;
		this->_clientsFd[i + 1].events = POLLIN;
	}
}

void	Server::waitInput(){
	int val = poll(_clientsFd, _clients.size() + 1, -1);
	std::cout << _clients.size() << std::endl;
	if (val < 0)
		std::cout << "error\n";
	for (int i = 0; _clientsFd[i].fd != 0; i++){
		if (_clientsFd[i].revents != 0){
			if (_clientsFd[i].fd == _sockfd)
				addClient();
		}
	}
}

void	Server::addClient()
{
	int	socket = 0;

	while (socket != -1)
	{
		struct	sockaddr_in address;
		socklen_t addrlen = sizeof(address);
		socket = accept(_sockfd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
		if (socket > 0)
			convertIP(&address.sin_addr);
	}
}

