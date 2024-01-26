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

    		std::vector<Client*>	&getClient(){return _clients;}
    		std::vector<Client*>	&getAdmins(){return _admins;}
		std::string getName()const {return _name;}
		std::string getTopic() const {return _topic;}
		bool	isAdmin(Client* client);
		void	setPass(std::string pass){_pass = pass;}
		void	join(Client* client);
		void	update(Client* client);
		void	sendMsg(Client* client, std::string target, std::string msg);
		void	sendMode(Client* client, std::string, std::string, std::string);
		void	parseMode(Client *client, std::string target, std::string modes);
		void	removeMode(Client *client,std::string, std::string);
		void	addMode(Client *client,std::string, std::string);
		void	setTopic(std::string topic) {_topic = topic;}

	private:
		Channel();
		Server*			_server;
		std::string		_name;
		std::string		_pass;
		bool			_i;
		bool			_t;
		bool			_l;
		bool			_k;
		std::string		_topic;
		Client*			_creator;
		std::vector<Client*> 	_admins;
		std::vector<Client*> 	_clients;
};