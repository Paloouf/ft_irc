#pragma once

#include "main.hpp"
class Server;
class Channel;

class Client{
	public:
		Client(Server* server, int fd, std::string hostname, int port);
		~Client();
		Client&	operator=(const Client& ref);

		void	addBuffer(char *buffer);
		void	parseBuffer(char *buffer);
		void	parseNego(char *buffer);
		void	parseMsg(char *buffer);
		void	resetBuffer();
		void	sendWelcome();

		//GETTERS
		int	getFd() const{return _fd;}
		std::string const &	getHostname() const{return _hostname;}
		int	getPort() const {return _port;}
		int getNego() const {return _negoCount;}
		Server*		getServer() {return _server;}
		std::string getNick() const {return _nick;}
		std::string getUser() const {return _user;}
		std::string getHost() const {return _hostname;}
		std::string getFullName() const {return _fullName;}
		std::string getCommand() const {return _command;}
		std::vector<Channel*> &getChan()  {return _chan;}

		//SETTERS
		void	setNego(const int nego){this->_negoCount = nego;}
		void	setNick(const std::string nick){this->_nick = nick;}
		void	setUser(const std::string user){this->_user = user;}
		void	setHost(const std::string hostname){this->_hostname = hostname;}
		void	setFullName(const std::string fullName){this->_fullName = fullName;}
	private:
		Server* _server;
		int	_fd;
		int	_negoCount;
		int	_port;
		std::string _fullName;
		std::string _user;
		std::string _nick;
		std::string _command;
		std::string _hostname;		
		std::vector<Channel*> _chan;	
};