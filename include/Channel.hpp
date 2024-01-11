#pragma once

#include "Server.hpp"
#include "Client.hpp"
#include "main.hpp"

class Server;
class Client;

class Channel{
	public:
		Channel(Server* server, std::string name);
		~Channel();

		std::string getName()const {return _name;}
	private:
		Channel();
		Server*	_server;
		std::string	_name;
		std::vector<Client*> _chanClients;
		
};