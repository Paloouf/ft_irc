#pragma once

#include "main.hpp"
class Server;
class Channel;

class Client{
	public:

		Client(Server* server, int fd, std::string hostname, int port);
		~Client();
		Client&	operator=(const Client& ref);
		void	sendBuffer(std::string buffer);
		void	resetBuffer();
		void	sendWelcome();
		void	resetSend();
		void	parseMsg(char *buffer);
		void	addBuffer(char *buffer);
		void	parseNego(char *buffer);
		void	parseBuffer(char *buffer);
		void	changeNick(std::string nick);
		void	pongReply(std::string buffer);
		void	privMsg(std::string command);
		void	changeTopic(std::string command);
		bool	checkNick(std::string nick);
		bool	checkDoubleNick(std::string nick);
		bool	checkDoubleUser(const char* user);

		//GETTERS
		int	getFd() const{return _fd;}
		int	getPort() const {return _port;}
		int getNego() const {return _negoCount;}
		std::string getFirstChannel() const;
		std::string getNick() const {return _nick;}
		std::string getUser() const {return _user;}
		std::string getSend() const {return _send;}
		std::string getPrefix() const {return _prefix;}
		std::string getHost() const {return _hostname;}
		std::string getCommand() const {return _command;}
		std::string getFullName() const {return _fullName;}
		std::string const &	getHostname() const{return _hostname;}
		Server*		getServer() {return _server;}
		std::vector<Channel*> &getChan()  {return _chan;}

		//SETTERS
		void	setPrefix();
		void	setNego(const int nego){this->_negoCount = nego;}
		void	setNick(const std::string nick){this->_nick = nick;}
		void	setUser(const std::string user){this->_user = user;}
		void	setHost(const std::string hostname){this->_hostname = hostname;}
		void	setFullName(const std::string fullName){this->_fullName = fullName;}

	private:

		Server* _server;
		int	_fd;	
		int	_port;
		int	_negoCount;
		std::string _send;
		std::string _user;
		std::string _nick;
		std::string _prefix;
		std::string _command;
		std::string _hostname;
		std::string _fullName;	
		std::vector<Channel*> _chan;	
};