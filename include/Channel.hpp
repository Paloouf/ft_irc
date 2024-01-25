#pragma once

#include "Server.hpp"
#include "Client.hpp"
#include "main.hpp"

class Server;
class Client;

class Channel{
	public:
		Channel(Server* server, std::string name, Client* client);
		~Channel();
		std::string getName()const {return _name;}
		std::vector<Client*>	&getClient(){return _clients;}
	private:
		Channel();
		Server*			_server;
		std::string		_name;
		Client*			_creator;
		std::vector<Client*> 	_admins;
		std::vector<Client*> 	_clients;
};