#include "../include/main.hpp"

Channel::Channel(Server *server, std::string name, Client* client) :_server(server), _name(name), _creator(client){
    std::cout << "New Channel " << _name << " created by client[" << _creator->getNick() << "]\n";
    setTopic("");
    _admins.push_back(client);
    _l = false;
    _k = false;
    _i = false;
    _t = false;
    _o = false;
    _limit = 0;
    std::string botname = "EasyBot!EasyBot@EasyRC.gg";
    client->sendBuffer(RPL_JOIN(botname, _name));
    this->join(client);
}

Channel::~Channel(){
	std::cout << "Channel '" << getName() << "' destroyed\n";
}

void Channel::join(Client* client){
    if (!_l || _limit >= _clients.size() + 1)
    {
        if (!_i || (_i && _invited.find(client->getNick()) != _invited.end()))
        {
            client->sendBuffer(RPL_JOIN(client->getPrefix(), _name));
            if (!_topic.compare(""))
                client->sendBuffer(RPL_NOTOPIC(client->getNick(), this->getName()));
            else
                client->sendBuffer(RPL_TOPIC(client->getNick(), this->getName(), this->getTopic()));
            _clients.push_back(client);
            client->getChan().push_back(this);
            std::string names;
            for (std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
            {
                for(std::vector<Client*>::iterator itt = _admins.begin(); itt != _admins.end(); itt++)
                {
                    if ((*itt)->getFd() == (*it)->getFd())
                        names += "@";
                }
                names += (*it)->getNick() + " ";
            }
            names += "@EasyBot";
            client->sendBuffer(RPL_NAMREPLY(client->getNick(), this->getName(), names.c_str()));
            client->sendBuffer(RPL_ENDOFNAMES(client->getNick(), this->getName()));
            update(client);
        }
        else
            client->sendBuffer(ERR_INVITEONLY(client->getPrefix(), getName()));
    }
    else
        client->sendBuffer(ERR_CHANNELISFULL(client->getPrefix(), getName()));
}

void	Channel::update(Client *client){
	for(std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end() - 1; it++){
		(*it)->sendBuffer(RPL_JOIN(client->getPrefix(), _name));
		std::cout << (*it)->getFd() << ": updated with " << RPL_JOIN(client->getPrefix(), _name);
	}
}

void	Channel::sendMsg(Client *client, std::string target, std::string msg){
	for(std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++){
		if (client->getFd() != (*it)->getFd()){
			(*it)->sendBuffer(SEND_PRIVMSG(client->getPrefix(), target, msg));
			std::cout << (*it)->getFd() << ": updated with " << SEND_PRIVMSG(client->getPrefix(), target, msg);
		}
	}
}

void	Channel::broadcast(std::string message)
{	
	for(std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++){
		(*it)->sendBuffer(message);
	}
}


void	Channel::sendMode(Client *client, std::string target, std::string mode, std::string msg){
	for(std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++){
			(*it)->sendBuffer(SEND_MODE(client->getPrefix(), target, mode, msg));
			std::cout << (*it)->getFd() << ": updated with " << SEND_MODE(client->getPrefix(), target, mode, msg);	
	}
}


void    Channel::parseMode(Client *client, std::string buff){
    std::stringstream smodes(buff);
	std::string modes, param;
	std::vector<std::string> params;
	smodes >> modes;
	std::getline(smodes, param, ' ');
	while (std::getline(smodes, param, ' '))
		params.push_back(param);
    if (isAdmin(client))
    {
        while (modes.find("-") != std::string::npos || modes.find("+") != std::string::npos)
        {
            if (modes.find("-") < modes.find("+"))
            {
                //remove modes
                size_t i = modes.find("-");
                std::string del;
                i++;
                while (i != modes.find("+") && i < modes.size()){
                    del += modes[i];
                    i++;
                }
                removeMode(client, del, params);
                modes = modes.substr(i);
            }
            else
            {
                //add modes
                size_t i = modes.find("+");
                std::string add;
                i++;
                while (i != modes.find("-") && i < modes.size()){
                    add += modes[i];
                    i++;
                }
                addMode(client, add, params);
                modes = modes.substr(i);
            }
        }
    }
    else
        std::cout << "Client is not admin\n";
}

void    Channel::removeMode(Client *client,std::string del, std::vector<std::string> params){
    int size = params.size();
    if (size > 0){
        params.back().resize(params.back().size() - 1);
    }
    char c;
    int i = 0;
    if (size > 0){
        std::vector<std::string>::iterator it = params.begin();
        while ((c = del[i])){
            switch(c){
                case 'i':{
                    _i = false;
                    broadcast(RPL_REMINV(client->getPrefix(), getName()));
                    break;
                }
                case 'l':{
                    _l = false;
                    broadcast(RPL_REMLIM(client->getPrefix(), getName()));
                    break;
                }
                case 'o':{
                    std::string msg = (*it);
                    for (int j = 0; j < size; j++){
                        for(std::vector<Client*>::iterator itt = _clients.begin(); itt != _clients.end(); itt++){
                            if ((*itt)->getNick() == (*it)){
                                if (this->isAdmin((*itt)) && _admins.size() > 1 && client->getNick() != (*it)){
                                    std::remove(_admins.begin(), _admins.end(), (*itt));
	 				                _admins.pop_back();
                                    broadcast(RPL_REMOP(client->getPrefix(), getName(), msg));
                                }
                            }
                        }
                        it++;
                    }
                    break;
                }
                case 'k':{
                    _k = false;
                    broadcast(RPL_REMPASS(client->getPrefix(), getName()));
                    break;
                }
                case 't':{
                    _t = false;
                    broadcast(RPL_REMTOP(client->getPrefix(), getName()));
                    break;
                }
                default:
                    break;
            }
            i++;
        }
    }
    else{
        while ((c = del[i])){
            switch(c){
                case 'i':{
                    _i = false;
                    broadcast(RPL_REMINV(client->getPrefix(), getName()));
                    break;
                }
                case 'l':{
                    _l = false;
                    broadcast(RPL_REMLIM(client->getPrefix(), getName()));
                    break;
                }
                case 'o':{
                    std::cout << "Mode -o, missing operator names\n";
                    break;
                }
                case 'k':{
                    _k = false;
                    broadcast(RPL_REMPASS(client->getPrefix(), getName()));
                    break;
                }
                case 't':{
                    _t = false;
                    broadcast(RPL_REMTOP(client->getPrefix(), getName()));
                    break;
                }
                default:
                    break;
            }
            i++;
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
    int size = params.size();
    if (size > 0){
        params.back().resize(params.back().size() - 1);
    }
    char    c;
    int i = 0;
    bool k = false;
    bool o = false;
    bool l = false;
    while ((c = add[i])){
        if (c == 'k'){
            if (k == false)
                k = true;
            //else
                //ERROR DUPLICATE MODE
        }
        if (c == 'o'){
            if (o == false)
                o = true;
            //else
                //ERROR DUPLICATE MODE
        }
        if (c == 'l'){
            if (l == false)
                l = true;
            //else
                //ERROR DUPLICATE MODE
        }
        i++;
    }
    i = 0;
    if (size > 0){
        std::vector<std::string>::iterator it = params.begin();
        while ((c = add[i]))
        {  
            switch(c){
                case 'k':{
                    std::cout << "Mode K\n";
                    _k = true;
                    setPass((*it));
                    broadcast(RPL_ADDPASS(client->getPrefix(), getName(), (*it)));
                    it++;
                    break;
                }
                case 'o':{
                    std::cout << "Mode O\n";
                    std::string msg = (*it);
                    for (int j = 0; j < size - l - k; j++){
                        for(std::vector<Client*>::iterator itt = _clients.begin(); itt != _clients.end(); itt++){
                            if ((*itt)->getNick() == (*it)){
                                if (!this->isAdmin((*itt))){
                                    _admins.push_back((*itt));
                                    broadcast(RPL_ADDOP(client->getPrefix(), getName(), msg));
                                }
                            }
                        }
                        it++;
                    }
                    break;
                }
                case 'i':{
                    if (!_i){
                        std::cout << "Mode i\n";
                        broadcast(RPL_ADDINV(client->getPrefix(), getName()));
                        _i = true;
                    }
                    break;
                }
                case 'l':{
                    std::cout << "Mode l " << (*it) << "\n";
                    setLimit(client, (*it));
                    it++;
                    _l = true;
                    break;
                }
                case 't':{
                    if (!_t){
                        std::cout << "Mode t\n";
                        broadcast(RPL_ADDTOP(client->getPrefix(), getName()));
                        _t = true;
                    }
                    break;
                }
                default:
                    break;
            }
            i++;
        }
    }
    else{
        while ((c = add[i]))
        {  
            switch(c){
                case 'k':{
                    std::cout << "Missing pass for mode k\n";
                    break;
                }
                case 'o':{
                    std::cout << "Missing operator names\n";
                    break;
                }
                case 'i':{
                    if (!_i){
                        std::cout << "Mode i\n";
                        broadcast(RPL_ADDINV(client->getPrefix(), getName()));
                        _i = true;
                    }
                    break;
                }
                case 'l':{
                    std::cout << "Missing limit argument\n";
                    break;
                }
                case 't':{
                    if (!_t){
                        std::cout << "Mode t\n";
                        broadcast(RPL_ADDTOP(client->getPrefix(), getName()));
                        _t = true;
                    }
                    break;
                }
                default:{
                    break;
                }
            }
            i++;
        }
    }
}

void    Channel::setLimit(Client *client, std::string limit){
    int i = 0;
    bool test = true;
    while (limit[i]){
        if (!isdigit(limit[i]))
        {
            std::cout << "probleme: " << limit[i] << std::endl;
            test = false;//ERROR BADNUMERICS
        }
        i++;
    }
    if (test){
        std::cout << "LIMIT SET TO " << limit << std::endl;
        _limit = std::atoi(limit.c_str());
        broadcast(RPL_ADDLIMIT(client->getPrefix(), getName(), limit));
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
            client->deleteChan(this);
			_clients.erase(_clients.begin() + i);
			i = 0;
		}
	}
	if (_clients.size() > 0 && _admins.size() == 0)
	{
		_admins.push_back(_clients[0]);
        std::string servname = "EasyRC.gg";
        broadcast(RPL_ADDOP(servname, getName(), _clients[0]->getNick()));
	}
	if (_clients.size() == 0){
		getServer()->deleteChannel(getName());
    }
}

