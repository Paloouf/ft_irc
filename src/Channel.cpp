#include "../include/main.hpp"

Channel::Channel(Server *server, std::string name, Client* client) :_server(server), _name(name), _creator(client){
    std::cout << "New Channel " << _name << " created by client[" << _creator->getFd() << "]\n";
    setTopic("");
	_admins.push_back(client);
    this->join(client);
}
Channel::~Channel(){}

void Channel::join(Client* client){
    std::string prefix = client->getNick() + (client->getUser().empty() ? "" : "!" + client->getUser()) + (client->getHostname().empty() ? "" : "@" + client->getHostname());
    std::string join = ":" + prefix + " JOIN " + _name + "\r\n";
    send(client->getFd(), join.c_str(), join.size(), 0);
    if (!_topic.compare(""))
    {
        std::string topic = RPL_NOTOPIC(client->getNick(), this->getName());
        send(client->getFd(), topic.c_str(), topic.size(), 0);
    }
    else{
        std::string topic = RPL_TOPIC(client->getNick(), this->getName(), this->getTopic());
        send(client->getFd(), topic.c_str(), topic.size(), 0);
    }
    _clients.push_back(client);
    client->getChan().push_back(this);
    std::string names;
    for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++){
        names += (*it)->getNick() + " ";
    }
    std::string userlist = RPL_NAMREPLY(client->getNick(), this->getName(), names.c_str());
    send(client->getFd(), userlist.c_str(), userlist.size(), 0);
    std::string endofnames = RPL_ENDOFNAMES(client->getNick(), this->getName());
    send(client->getFd(), endofnames.c_str(), endofnames.size(), 0);

	std::cout << join << userlist << endofnames << std::endl;
    //Need to make more functions to send /PRIVMSGS to other clients in the channel, set Topic with /TOPIC
}

void	Channel::update(Client *client){
	std::string prefix = client->getNick() + (client->getUser().empty() ? "" : "!" + client->getUser()) + (client->getHostname().empty() ? "" : "@" + client->getHostname());
	std::string join = ":" + prefix + " JOIN " + _name + "\r\n";
	for(std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end() - 1; it++){
		send((*it)->getFd(), join.c_str(), join.size(), 0);
		std::cout << (*it)->getFd() << ": updated with " << join;
	}

}

void	Channel::sendMsg(Client *client, std::string target, std::string msg){
	std::string prefix = client->getNick() + (client->getUser().empty() ? "" : "!" + client->getUser()) + (client->getHostname().empty() ? "" : "@" + client->getHostname());
    	std::string fullmsg = ":" + prefix + " PRIVMSG " + target + " :" + msg;
	for(std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++){
		if (client->getFd() != (*it)->getFd()){
			send((*it)->getFd(), fullmsg.c_str(), fullmsg.size(), 0);
			std::cout << (*it)->getFd() << ": updated with " << fullmsg;
		}
	}
}


void	Channel::deleteUser(Client *client)
{
	for (unsigned i = 0; i < _admins.size(); i++)
	{
		if (_admins[i]->getFd() == client->getFd())
		{
			_admins.erase(_admins.begin() + i);
			i = 0;
		}
	}
	for (unsigned i = 0; i < _clients.size(); i++)
	{
		if (_clients[i]->getFd() == client->getFd())
		{
			_clients.erase(_clients.begin() + i);
			i = 0;
		}
	}
	if (_clients.size() > 0 && _admins.size() == 0)
	{
		_admins.push_back(_clients[0]);
		//need to add sendmsg fonction for tell at all client the new admin in chan
	}
}

