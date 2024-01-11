#pragma once

#include "Server.hpp"
#include "Client.hpp"
#include "main.hpp"

class Server;
class Client;

class Channel{
	public:
		Channel(Server* server, Client* creator, std::string name);
		~Channel();

		std::string getName()const {return _name;}
	private:
		Channel();
		Server*			_server;
		std::string		_name;
		Client*			_creator;
		std::vector<Client*> 	_admins;
		std::vector<Client*> 	_clients;
};
