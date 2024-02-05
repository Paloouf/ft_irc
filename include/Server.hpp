#pragma once

#include "main.hpp"
#include <map>
class Client;
class Channel;

class Server{
	private:
	
		Server(){};

		//VARIABLES//
		int	_sockfd;
		std::string _port;
		std::string _password;
		std::string _date;
		std::vector<Client*> _clients;
		std::map<std::string, Channel*> _chanMap;
		struct pollfd	*_clientsFd;
		
		//FUNCTIONS//
		void	setTime();
		void	createFd();
		void	listening();
		void	waitInput();
		void	addClient();
		void	receiveData(Client *client);
		void	sendWelcome(Client* client);
		void	parseBuffer(char* buffer, Client* client);
		std::string	convertIP(const void *address);
		

		//void	createClient();
	public:

		Server(std::string port, std::string password);
		~Server();
		
		void							checkInput();
		void							deleteClient(Client* client);
		void							broadcast(std::string message);
		void							replyUser(Client* client, char* buffer);
		void							whoReply(Client* client, char* buffer);
		void							replyChannel(Client* client, char* buffer);
		void							checkChannel(Client* client, std::string buffer);
		std::string 					getDate(){return _date;}
		std::string 					getPort(){return _port;}
		std::string 					getPassword(){return _password;}
		std::vector<Client*>			&getClient(){return _clients;}
		std::map<std::string, Channel*>	&getChan(){return _chanMap;}
};
