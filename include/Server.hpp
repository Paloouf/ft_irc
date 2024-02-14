#pragma once

#include "main.hpp"
#include <map>
class Client;
class Channel;

class Server{
	private:
		Server(){};
		
		int								_sockfd;
		std::string 					_port;
		std::string 					_password;
		std::string 					_date;
		std::vector<Client*> 			_clients;
		std::map<std::string, Channel*> _chanMap;
		struct pollfd					*_clientsFd;
		
		void			parseBuffer(char* buffer, Client* client);
		void			sendWelcome(Client* client);
		void			listening();
		void			createFd();
		void			waitInput();
		void			addClient();
		void			setTime();
		void			receiveData(Client *client);
		std::string		convertIP(const void *address);

	public:
		~Server();
		Server(std::string port, std::string password);

		void							checkInput();
		void							deleteClient(Client* client);
		void							checkChannel(Client* client, std::string buffer);
		void							whoReply(Client* client, char* buffer);
		void							replyChannel(Client* client, char* buffer);
		void							replyUser(Client* client, char* buffer);
    	void							deleteChannel(std::string);
		void							broadcast(std::string message);

		//GETTERS
		std::string 					getDate()		{return _date;}
		std::string 					getPassword()	{return _password;}
		std::string 					getPort()		{return _port;}
		std::vector<Client*>			&getClient()	{return _clients;}
		std::map<std::string, Channel*>	&getChan()		{return _chanMap;}

};
