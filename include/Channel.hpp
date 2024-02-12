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
		void	sendMsg(Client* client, std::string target, std::string msg);
		void	sendMode(Client* client, std::string, std::string, std::string);
		void	parseMode(Client *client, std::string modes);
		void	removeMode(Client *client,std::string, std::vector<std::string>);
		void	addMode(Client *client,std::string, std::vector<std::string>);
		void	broadcast(std::string message);
		void	deleteUser(Client *client);
		bool	isAdmin(Client* client);

		//GETTERS
    	std::vector<Client*> &getClient()				{return _clients;}
    	std::vector<Client*> &getAdmins()				{return _admins;}
		std::map<std::string, Client*> &getInvited()	{return _invited;}
		Server*		getServer() 						{return _server;}
		std::string getName()const 						{return _name;}
		std::string getTopic() const 					{return _topic;}
		std::string getPass() const 					{return _pass;}
		bool	const &getChanK() const 				{return _k;}


		//SETTERS
		void	setLimit(Client*, std::string);
		void	setPass(std::string pass)	{_pass = pass;}
		void	setTopic(std::string topic) {_topic = topic;}

	private:
		Channel();
		Server*							_server;
		std::string						_name;
		std::string						_pass;
		bool							_i;
		bool							_o;
		bool							_t;
		bool							_l;
		bool							_k;
		std::string						_topic;
		long unsigned int				_limit;

		Client*							_creator;
		std::vector<Client*> 			_admins;
		std::vector<Client*> 			_clients;
		std::map<std::string, Client*>	_invited;

};

