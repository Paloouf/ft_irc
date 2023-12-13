#pragma once

#include "main.hpp"
class Server;

class Client{
	public:
		Client(Server* server, int fd, std::string hostname, int port);
		~Client();
		Client&	operator=(const Client& ref);

		//GETTERS
		int	getFd() const{return _fd;}
		std::string const &	getHostname() const{return _hostname;}
		int	getPort() const {return _port;}
	private:
		Server* _server;
		int	_fd;
		std::string _hostname;
		int	_port;	
		
};