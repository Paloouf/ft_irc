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
		std::string getNick() const {return _user;}
		std::string getHost() const {return _hostname;}

		//SETTERS
		void	setIP(const std::string & ipClient){this->_ipClient = ipClient;}
		void	setNick(const std::string nick){this->_nick = nick;}
		void	setUser(const std::string user){this->_user = user;}
		void	setHost(const std::string hostname){this->_hostname = hostname;}
	private:
		Server* _server;
		int	_fd;
		std::string _hostname;
		int	_port;	
		std::string _ipClient;
		std::string _user;
		std::string _nick;
		std::vector<Channel*> _chan;
		
};