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
		void	join(Client* client);
	private:
		Channel();
		Server*			_server;
		std::string		_name;
		std::string		_topic;
		Client*			_creator;
		std::vector<Client*> 	_admins;
		std::vector<Client*> 	_clients;
};