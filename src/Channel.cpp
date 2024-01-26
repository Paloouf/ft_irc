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
    smodes >> modes >> param;
    if (modes.find("-") != std::string::npos){
        //remove modes
        size_t i = modes.find("-");
        std::string del;
        i++;
        while (i != modes.find("+") && i < modes.size()){
            del += modes[i];
            i++;
        }
        removeMode(client, del, param);
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
        addMode(client, add, param);
        std::cout << target << " miaou " << client->getFd() << std::endl;
    }
}

void    Channel::removeMode(Client *client,std::string del, std::string param){
    std::cout << del;
    bool    isAdmin = false;
    for (std::vector<Client*>::iterator it = _admins.begin(); it != _admins.end(); it++){
        if ((*it)->getFd() == client->getFd())
            isAdmin = true;
    }
    if (isAdmin){
        if (del.find("i") != std::string::npos)
            _i = false;
        if (del.find("t") != std::string::npos)
            _t = false;
        if (del.find("k") != std::string::npos)
            _k = false;
        if (del.find("o") != std::string::npos){
            std::cout << "Removed operator privileges for : " << param;
        }
        if (del.find("l") != std::string::npos)
            _l = false;
    }
}

bool    Channel::isAdmin(Client* client){
    bool    isAdmin = false;
    for (std::vector<Client*>::iterator it = _admins.begin(); it != _admins.end(); it++){
        if ((*it)->getFd() == client->getFd())
            isAdmin = true;
    }
    return (isAdmin);
}

void    Channel::addMode(Client *client,std::string add, std::string param){
    std::cout << add << param;
    if (this->isAdmin(client)){
        if (add.find("i") != std::string::npos)
            _i = true;
        if (add.find("t") != std::string::npos)
            _t = true;
        if (add.find("k") != std::string::npos){
            std::cout << "New pass for " << this->getName() << " is " << param << std::endl;
            _k = true;
            setPass(param);
        }
        if (add.find("o") != std::string::npos){
            std::cout << "Added operator privileges for : " << param;
            std::string msg = param + "\n";
            for(std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++){
                if ((*it)->getNick() == param){
                    if (!this->isAdmin((*it))){
                        _admins.push_back((*it));
                        sendMode(client, getName(), "+o", msg);
                    }
                    else
                        std::cout << (*it)->getNick() << " is already admin\n";
                }
                else
                    std::cout << param << " doesn't match any username\n";
            }  
        }
        if (add.find("l") != std::string::npos)
            _l = true;
    }
}