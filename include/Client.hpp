#pragma once

#include "main.hpp"
class Server;
class Channel;

class Client{
	public:
		Client(Server* server, int fd, std::string hostname, int port);
		~Client();

		void	invite(std::string command);
		void	kick(std::string command);
		void	part(std::string command);
		void	mode(std::string command);
		void	errorReset(const char* error);
		void	sendBuffer(std::string buffer);
		void	resetSend();
		void	sendWelcome();
		void	addBuffer(char *buffer);
		void	resetBuffer();
		void	parseBuffer(char *buffer);
		void	parseNego(char *buffer);
		void	parseMsg(char *buffer);
		void	changeNick(std::string nick);
		void	pongReply(std::string buffer);
		void	privMsg(std::string command);
		void	changeTopic(std::string command);
		void	deleteChan(Channel *channel);
		bool	checkNick(std::string nick);
		bool	checkDoubleNick(std::string nick);
		bool	checkDoubleUser(const char* user);
		void	help();


		//GETTERS
		int	getFd() const						{return _fd;}
		int	getPort() const 					{return _port;}
		int getNego() const 					{return _negoCount;}
		Server*		getServer() 				{return _server;}
		std::string getNick() const 			{return _nick;}
		std::string getUser() const 			{return _user;}
		std::string getSend() const 			{return _send;}
		std::string getHost() const 			{return _hostname;}
		std::string getFullName() const 		{return _fullName;}
		std::string getCommand() const 			{return _command;}
		std::string getPrefix() const 			{return _prefix;}
		std::string const &	getHostname() const	{return _hostname;}
		std::vector<Channel*> &getChan()  		{return _chan;}
   		std::string getFirstChannel() const;


		//SETTERS
		void	setNego(const int nego)					{this->_negoCount = nego;}
		void	setNick(const std::string nick)			{this->_nick = nick;}
		void	setUser(const std::string user)			{this->_user = user;}
		void	setHost(const std::string hostname)		{this->_hostname = hostname;}
		void	setFullName(const std::string fullName)	{this->_fullName = fullName;}
		void	setPrefix();

	private:
		Server*					_server;
		int						_fd;
		int						_negoCount;
		int						_port;
		std::string				_send;
		std::string				_fullName;
		std::string				_user;
		std::string				_nick;
		std::string				_prefix;
		std::string				_command;
		std::string				_hostname;		
		std::vector<Channel*>	_chan;
};