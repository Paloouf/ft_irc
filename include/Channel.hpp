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

		void	join(Client* client);
		void	update(Client* client);
		bool	isAdmin(Client* client);
		void	deleteUser(Client *client);
		void	broadcast(std::string message);
		void	sendMsg(Client* client, std::string target, std::string msg);
		void	sendMode(Client* client, std::string, std::string, std::string);
		void	parseMode(Client *client, std::string target, std::string modes);
		void	removeMode(Client *client,std::string, std::vector<std::string>);
		void	addMode(Client *client,std::string, std::vector<std::string>);
		
		//GETTER
		std::string getName()const {return _name;}	
		std::string getPass() const {return _pass;}
		std::string getTopic() const {return _topic;}
    	std::vector<Client*>	&getClient(){return _clients;}
    	std::vector<Client*>	&getAdmins(){return _admins;}

		//SETTERS
		void	setPass(std::string pass){_pass = pass;}
		void	setTopic(std::string topic) {_topic = topic;}

	private:
		Channel();
		Server*			_server;
		std::string		_name;
		std::string		_pass;
		std::string		_topic;
		Client*			_creator;		
		std::vector<Client*> 	_admins;
		std::vector<Client*> 	_clients;

		bool			_i;
		bool			_t;
		bool			_l;
		bool			_k;
};

