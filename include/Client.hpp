#pragma once

#include "main.hpp"
class Server;
class Channel;

class Client{
	public:
		Client(Server* server, int fd, std::string hostname, int port);
		~Client();
		Client&	operator=(const Client& ref);

		//GETTERS
		int	getFd() const{return _fd;}
		std::string const &	getHostname() const{return _hostname;}
		int	getPort() const {return _port;}

		//SETTERS
		void	setIP(const std::string & ipClient){this->_ipClient = ipClient;}
		void	setNick(const std::string nick){this->_nick = nick;}
	private:
		Server* _server;
		int	_fd;
		std::string _hostname;
		int	_port;	
		std::string _ipClient;
		std::string _nick;
		std::vector<Channel*> _chan;
		
};