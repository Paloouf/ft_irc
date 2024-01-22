#include "../include/main.hpp"

Channel::Channel(Server *server, std::string name, Client* client) :_server(server), _name(name), _creator(client){
    std::cout << "New Channel " << _name << " created by client[" << _creator->getFd() << "]\n";
    std::string join = "JOIN " + _creator->getNick() + " " + _name + "\r\n";
    send(_creator->getFd(), join.c_str(), join.size(), 0);
    this->join(client);
}
Channel::~Channel(){}

void Channel::join(Client* client){
	//Send RPL_Topic and RPL_NAMREPLY
	
	std::cout << "pipouet: " << client->getFd() << std::endl;
}