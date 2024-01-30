#include "../include/main.hpp"

Channel::Channel(Server *server, std::string name, Client* client) :_server(server), _name(name), _creator(client){
    std::cout << "New Channel " << _name << " created by client[" << _creator->getFd() << "]\n";
    setTopic("");
    _admins.push_back(client);
    this->join(client);
    _l = false;
    _k = false;
    _i = false;
    _t = false;
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
	for(std::vector<Client*>::iterator itt = _admins.begin(); itt != _admins.end(); itt++){
		if ((*itt)->getFd() == (*it)->getFd()){
			names += "@";
			//it++;
		}
	}
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


void	Channel::broadcast(std::string message)
{
	for(std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
		send((*it)->getFd(), message.c_str(), message.size(), 0);
}


void	Channel::sendMode(Client *client, std::string target, std::string mode, std::string msg){
	std::string prefix = client->getNick() + (client->getUser().empty() ? "" : "!" + client->getUser()) + (client->getHostname().empty() ? "" : "@" + client->getHostname());
    	std::string fullmsg = ":" + prefix + " MODE " + target + " " + mode + " " + msg;
	for(std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++){
			send((*it)->getFd(), fullmsg.c_str(), fullmsg.size(), 0);
			std::cout << (*it)->getFd() << ": updated with " << fullmsg;
		
	}
}

void    Channel::parseMode(Client *client, std::string target, std::string buff){
	std::stringstream smodes(buff);
	std::string modes, param;
	std::vector<std::string> params;
	smodes >> modes;
	std::getline(smodes, param, ' ');
	while (std::getline(smodes, param, ' ')){
		params.push_back(param);
	}
	if (modes.find("-") != std::string::npos){
		//remove modes
		size_t i = modes.find("-");
		std::string del;
		i++;
		while (i != modes.find("+") && i < modes.size()){
		del += modes[i];
		i++;
		}
		removeMode(client, del, params);
	}
	if (modes.find("+") != std::string::npos){
		//add modes
		size_t i = modes.find("+");
		std::cout << "i: " << i << ", size: " << modes.size() << std::endl;
		std::string add;
		i++;
		while (i != modes.find("-") && i < modes.size()){
		add += modes[i];
		i++;
		}
		addMode(client, add, params);
		std::cout << target << " miaou " << client->getFd() << std::endl;
	}
}

void    Channel::removeMode(Client *client,std::string del, std::vector<std::string> params){
	for (std::vector<std::string>::iterator it = params.begin(); it != params.end(); it++){
		std::string param = (*it);
		if (this->isAdmin(client)){
			if (del.find("i") != std::string::npos)
			_i = false;
			if (del.find("t") != std::string::npos)
			_t = false;
			if (del.find("k") != std::string::npos){
				_k = false;
				broadcast(RPL_REMPASS(client->getPrefix(), getName()));
			}
			if (del.find("o") != std::string::npos){
			std::cout << "Removed operator privileges for : " << param << std::endl;
			std::string msg = param + "\n";
			for(std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++){
				if ((*it)->getNick() + "\n" == param || (*it)->getNick() == param){
				if (this->isAdmin((*it)) && _admins.size() > 1 && client->getNick() != param){
					std::remove(_admins.begin(), _admins.end(), (*it));
					_admins.pop_back();
					std::cout << "minou\n";
					sendMode(client, getName(), "-o", msg);
					//broadcast(RPL_REMOP(client->getPrefix(), getName(), msg));
				}
				else
					std::cout << (*it)->getNick() << " is not admin\n";
				}
			} 
			}
			if (del.find("l") != std::string::npos)
			_l = false;
		}
	}
}

bool    Channel::isAdmin(Client* client){
    for (std::vector<Client*>::iterator it = _admins.begin(); it != _admins.end(); it++){
        if ((*it)->getFd() == client->getFd())
            return true;
    }
    return (false);
}

void    Channel::addMode(Client *client,std::string add, std::vector<std::string> params){
	//PROBLEME SI PAS D'ARGUMENTS DERRIERE
	for (std::vector<std::string>::iterator it = params.begin(); it != params.end(); it++){
		std::string param = (*it);
		if (this->isAdmin(client)){
			if (add.find("i") != std::string::npos)
			_i = true;
			if (add.find("t") != std::string::npos)
			_t = true;
			if (add.find("o") < add.find("k") && add.find("k") != std::string::npos){
				std::cout << "MAIAAAOIWIOFAWIFOIFAOINFOAN\n";
				if (param[param.size() - 1] == '\n'){
					std::cout << "New pass for " << this->getName() << " is " << param << std::endl;
					_k = true;
					setPass(param);
					broadcast(RPL_NEWPASS(client->getPrefix(), getName(), getPass()));
				}
				else{
					std::string msg = param + "\n";
					for(std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++){
						std::cout << "Nique: " << (*it)->getNick() << "nicksize: " << param.size() << std::endl;
						if ((*it)->getNick() + "\n" == param || (*it)->getNick() == param){
							if (!this->isAdmin((*it))){
								_admins.push_back((*it));
								//sendMode(client, getName(), "+o", msg);
								broadcast(RPL_ADDOP(client->getPrefix(), getName(), msg));
							}
						}
						else
							std::cout << (*it)->getNick() << " is already admin\n";
						
					} 
				}
			}
			if (add.find("k") != std::string::npos && add.find("o") == std::string::npos){
			std::cout << "New pass for " << this->getName() << " is " << param << std::endl;
			_k = true;
			setPass(param);
			broadcast(RPL_NEWPASS(client->getPrefix(), getName(), getPass()));
			}
			if (add.find("o") != std::string::npos && add.find("k") == std::string::npos){
			std::cout << "Added operator privileges for : " << param;
			std::string msg = param + "\n";
			for(std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++){
				std::cout << "Nique: " << (*it)->getNick() << "nicksize: " << param.size() << std::endl;
				if ((*it)->getNick() + "\n" == param || (*it)->getNick() == param){
				if (!this->isAdmin((*it))){
					_admins.push_back((*it));
					//sendMode(client, getName(), "+o", msg);
					broadcast(RPL_ADDOP(client->getPrefix(), getName(), msg));
				}
				else
					std::cout << (*it)->getNick() << " is already admin\n";
				}
			}  
			}
			if (add.find("k") < add.find("o") && add.find("o") != std::string::npos){
				if (it == params.begin()){
					std::cout << "New pass for " << this->getName() << " is " << param << std::endl;
					_k = true;
					setPass(param);
					broadcast(RPL_NEWPASS(client->getPrefix(), getName(), getPass()));
				}
				else{
					std::cout << "Added operator privileges for : " << param;
					std::string msg = param + "\n";
					for(std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++){
						std::cout << "Nique: " << (*it)->getNick() << "nicksize: " << param.size() << std::endl;
						if ((*it)->getNick() + "\n" == param || (*it)->getNick() == param){
							if (!this->isAdmin((*it))){
								_admins.push_back((*it));
								//sendMode(client, getName(), "+o", msg);
								broadcast(RPL_ADDOP(client->getPrefix(), getName(), msg));
							}
							else
								std::cout << (*it)->getNick() << " is already admin\n";
						}
					} 
				}
			}
			if (add.find("l") != std::string::npos)
			_l = true;
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

