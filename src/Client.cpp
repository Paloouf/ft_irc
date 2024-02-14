#include "../include/Client.hpp"

Client::Client(Server *server, int fd, std::string hostname, int port) :_server(server), _fd(fd), _port(port),_hostname(hostname){
	std::cout << &_server << " Test client, fd: " << this->getFd() << ", hostname: " << this->getHostname() << std::flush;
	std::cout << ", port: " << this->getPort() << std::endl << std::endl;
	this->resetBuffer();	
	_negoCount = 0;
}

Client::~Client()
{
	std::cout << "Client destructor called\n";
}

void	Client::addBuffer(char* buffer)
{
	_command.append(buffer);
}

void	Client::resetBuffer()
{
	_command.clear();
}

void	Client::errorReset(const char* error)
{
	resetBuffer();
	_server->deleteClient(this);
	std::cerr << error << std::endl;
}

void	Client::parseBuffer(char * buffer)
{
	std::string command = buffer;
	if (command.find("\nQUIT ") != std::string::npos){
			getServer()->deleteClient(this);
			return ;
		}
	if (_negoCount < 4)
	{
		
		try
			{parseNego(buffer);}
		catch (Error::wrongPassword& error)
			{errorReset(error.what());}
		catch (Error::wrongArgument& error)
			{errorReset(error.what());}
		catch (Error::wrongNickname& error)
			{errorReset(error.what());}
		catch (Error::usedNickname& error)
			{errorReset(error.what());}
		catch (Error::usedUsername& error)
			{errorReset(error.what());}	
	}
	else
		parseMsg(buffer);
}


void	Client::parseNego(char *buffer)
{
	std::string command = buffer;
	addBuffer(buffer);
	std::stringstream 	sBuff(getCommand());
	std::string			message;
	while (getline(sBuff, command))
	{
		std::cerr << "Negotiation step : Message from client " << getFd() << " : " << command << std::endl;
		if (command.size() > 3 && command.substr(0,3) == "CAP" && getNego() == 0)
		{
			message = "CAP * LS :\n";
			sendBuffer(message);
			std::cout << "Responding to client " << getFd() << " with message " << message;
			setNego(1);
		}
		else if (command.size() > 4 && command.substr(0,4) == "PASS" && getNego() == 1)
		{
			sBuff >> command;
			if (!command[0])
			{
				std::cout << "Responding to client " << getFd() << " with message " << ERR_NEEDMOREPARAMS(getHostname(), "PASS");
				sendBuffer(ERR_NEEDMOREPARAMS(getHostname(), "PASS"));
			}
			if (command != "PASS :" + _server->getPassword())
			{
				std::cout << "Responding to client " << getFd() << " with message " << ERR_PASSWDMISMATCH(getHostname());
				sendBuffer(ERR_PASSWDMISMATCH(getHostname()));
				throw Error::wrongPassword();
			}
			setNego(2);
		}
		else if (command.size() > 4 && command.substr(0,4) == "NICK" && getNego() == 2) 
		{
			if(!checkNick(command.substr(5)))
				throw Error::wrongNickname();
			if(!checkDoubleNick(command.substr(5)))
				throw Error::usedNickname();
			setNick(command.substr(5));
			setNego(3);
		}
		else if (command.size() > 4 && command.substr(0,4) == "USER" && getNego() == 3)
		{
			char* commandbis = &command[5];
			if(!checkDoubleUser(commandbis))
				throw Error::usedUsername();
			setUser(strtok(commandbis, " "));
			setFullName(command.substr(command.find(":") + 1));
			setPrefix();
			sendWelcome();
			setNego(4);
		}
		else
			throw Error::wrongArgument();
	}
	resetBuffer();
}

void	Client::setPrefix(){
	std::string prefix = this->getNick() + (this->getUser().empty() ? "" : "!" + this->getUser()) 
		+ (this->getHostname().empty() ? "" : "@" + this->getHostname());
	_prefix = prefix;
}

void	Client::parseMsg(char *buffer)
{
	std::string command = buffer;

	std::cout << "MSG_RCVD[" << getNick() << "]:" << command << std::endl;
	if (command.size() > 4 && command.substr(0,5) == "PING ")
		pongReply(command.substr(5));
	if (command.size() > 4 && command.substr(0,5) == "JOIN ")
		getServer()->checkChannel(this, command.substr(5, command.size() - 6));
	if (command.size() > 3 && command.substr(0,4) == "WHO ")
		getServer()->whoReply(this, buffer);
	if (command.size() > 4 && command.substr(0,5) == "NICK ")
		changeNick(command.substr(5, command.size() - 6));
	if (command.substr(0,8) == "PRIVMSG ")
		privMsg(command);
	if(command.substr(0,5) == "MODE ")
		mode(command);
	if(command.substr(0,7) == "TOPIC #")
		changeTopic(command.substr(6));
	if (command.substr(0,5) == "QUIT ")
		_server->deleteClient(this);
	if (command.substr(0,5) == "PART ")
		part(command);
	if (command.substr(0,5) == "KICK ")
		kick(command);
	if (command.substr(0,7) == "INVITE ")
		invite(command);
	if (command.substr(0,5) == "HELP\n" || command.substr(0,5) == "help\n")
		help();
}

void	Client::help()
{
	std::string msg = ":EasyBot PRIVMSG " + getNick() + " :Hello, I'm EasyBot and I'm here to help\n";
	sendBuffer(msg);
	msg = ":EasyBot PRIVMSG " + getNick() + " :Functions you can use now: /JOIN, /NICK, /PRIVMSG\n";
	sendBuffer(msg);
	msg = ":EasyBot PRIVMSG " + getNick() + " :Functions you can use when operator: /KICK, /MODE (i,t,k,o,l)\n";
	sendBuffer(msg);
	msg = ":EasyBot PRIVMSG " + getNick() + " :Functions you can use when member of a channel: /INVITE, /TOPIC\n";
	sendBuffer(msg);
}

void	Client::invite(std::string command)
{
	std::stringstream buff(command);
	std::string cmd, target, channelName;
	buff >> cmd >> target >> channelName;
	for(std::vector<Client*>::iterator it = getServer()->getClient().begin(); it != getServer()->getClient().end();it++){
		if ((*it)->getNick() == target){
			for(std::vector<Channel*>::iterator chanIt = _chan.begin(); chanIt != _chan.end(); chanIt++){
				if ((*chanIt)->getName() == channelName){
					(*it)->sendBuffer(RPL_INVITING(getPrefix(), target, channelName));
					(*chanIt)->getInvited().insert(make_pair(target, *it));
					return ;
				}
			}
			sendBuffer(ERR_NOTONCHANNEL(getPrefix(), channelName));
		}
	}
}

void	Client::kick(std::string command)
{
	std::string target = command.substr(command.find("#"));
	std::cout << target << std::endl;
	std::stringstream buff;
	buff << target;
	std::string	user, cible, channel;
	buff >> channel >> cible;
	user = this->getNick();
	std::cout << "user:" << user << " chan: " << channel << " cible:" << cible << std::endl;
	for (std::vector<Channel*>::iterator it = _chan.begin(); it != _chan.end(); it++){
		if (channel == (*it)->getName()){
			if ((*it)->isAdmin(this))
			{
				std::vector<Client*>::iterator itt = (*it)->getClient().begin();
				while (itt != (*it)->getClient().end())
				{
					if (cible == (*itt)->getNick())
					{
						(*it)->broadcast(RPL_KICK(this->getPrefix(), (*it)->getName(), (*itt)->getNick()));
						std::cout << "on broadcast: " << RPL_KICK(getPrefix(), (*it)->getName(), (*itt)->getNick());
						(*it)->deleteUser(*itt);
						if ((*it)->getClient().size() == 0)
							return;
						itt = (*it)->getClient().begin();
					}
					// else
					// 	std::cout << "pipounet" << std::endl;
					itt++;
				}
			}
			else
				std::cout << "This Client is not admin\n";
		}
		if (_chan.size() == 0)
			return;
	}
}

void	Client::part(std::string command)
{
	std::string target = command.substr(command.find("#"), command.find(" "));
	std::cout << target << std::endl;
	for (std::vector<Channel*>::iterator it = _chan.begin(); it != _chan.end(); it++){
		if (target == (*it)->getName()){
			(*it)->broadcast(RPL_PART(getPrefix(), (*it)->getName()));
			std::remove(_chan.begin(), _chan.end(), (*it));
			_chan.pop_back();
			(*it)->deleteUser(this);
			break;
		}
	}
}

void	Client::mode(std::string command)
{
	char* commandbis = &command[5];
	std::string target = strtok(commandbis, " ");
	if (target[0] == '#'){
		for (std::vector<Channel*>::iterator it = _chan.begin(); it != _chan.end(); it++){
			if ((*it)->getName() == target){
				(*it)->parseMode(this, command.substr(5 + target.size()));
			}
		}
	}
}

std::string	Client::getFirstChannel() const
{
	if(_chan.empty())
		return ("*");
	return (_chan[0]->getName());
}

bool		Client::checkNick(std::string nick)
{
	if (nick[0] == '#' || nick[0] == '&' || nick[0] == ':' || nick[0] == '@' || nick[0] == '!' || nick.find(' ') != std::string::npos)
	{
		sendBuffer(ERR_ERRONEUSNICKNAME(getHostname(), nick));
		return false;
	}	
	return true;
}

bool		Client::checkDoubleNick(std::string nick)
{
	for (std::vector<Client*>::iterator it = getServer()->getClient().begin(); it != getServer()->getClient().end(); it++)
	{
		if((*it)->getNick() == nick)
		{
			sendBuffer(ERR_NICKNAMEINUSE(getHostname(),nick));
			return false;
		}
	}
	return true;
}

bool		Client::checkDoubleUser(const char* user)
{
	for (std::vector<Client*>::iterator it = getServer()->getClient().begin(); it != getServer()->getClient().end(); it++)
	{
		if((*it)->getUser() == user)
		{				
			sendBuffer(ERR_ALREADYREGISTERED(getHostname()));
			return false;
		}
	}
	return true;
}

void	Client::sendWelcome()
{
	sendBuffer(RPL_WELCOME(getNick(), getFullName()));
	std::cout << "Responding to client " << getFd() << " with message " << RPL_WELCOME(getNick(), getFullName());
	sendBuffer(RPL_YOURHOST(getNick()));
	std::cout << "Responding to client " << getFd() << " with message " << RPL_YOURHOST(getNick());
	sendBuffer(RPL_CREATED(getNick(), _server->getDate()));
	std::cout << "Responding to client " << getFd() << " with message " << RPL_CREATED(getNick(), _server->getDate());
	sendBuffer(RPL_MYINFO(getNick()));
	std::cout << "Responding to client " << getFd() << " with message " << RPL_MYINFO(getNick());
	std::cout << "Successfully registered client " << getHostname() << std::endl << std::endl;
}

void	Client::pongReply(std::string buffer)
{
		std::cout << "Getting Ping request from client " << getFd() << std::endl;
		std::cout << "Responding to ping request from client " << getFd() << " with message " << PONG(buffer) << std::endl;
		sendBuffer(PONG(buffer));
}

void	Client::changeNick(std::string nick)
{
		if(checkNick(nick) && checkDoubleNick(nick))
		{
			std::cout << "Sending nickname change broadcast : " << NEW_NICK(getNick(), nick);
			getServer()->broadcast(NEW_NICK(getNick(), nick));
			setNick(nick);
		}
}

void	Client::privMsg(std::string command)
{
	char* commandbis = &command[8];
	std::string target = strtok(commandbis, " ");
	if (target[0] == '#'){
		for (std::vector<Channel*>::iterator it = _chan.begin(); it != _chan.end(); it++){
			if ((*it)->getName() == target){
				(*it)->sendMsg(this, target, command.substr(command.find(":") + 1));
			}
		}
	}
	else{
		std::string cmd, msg, cible;
		if (command.find("DCC") != std::string::npos)
		{
			std::stringstream buff;
			std::string ignore, file, sendKeyword, ip, port, size;
			buff << command;
			buff >> cmd >> ignore >> sendKeyword >> file >> ip >> port >> size;
			std::string user = ignore.substr(0, ignore.find(":") - 1);

			std::cout << "DEMANDE DE FILE TRANSFER: " << user << " " << command << "\n";
			for (std::vector<Client*>::iterator it = _server->getClient().begin(); it != _server->getClient().end(); it++){
					if ((*it)->getNick() == user){
						std::string reply = ":" + (*it)->getPrefix() + " PRIVMSG " + getNick() + " :\x01 DCC SEND " + file + " " + ip + " " + port + "\x01";
						std::cout << reply;
						reply[reply.size() - 2] = '\0';
						(*it)->sendBuffer(reply);
						std::string reply2 = ":" + (*it)->getPrefix() + " NOTICE " + (*it)->getNick() + " :\x01 DCC SEND " + file + " " + ip + " " + port + "\x01";
						std::cout << reply2;
						reply2[reply2.size() - 2] = '\0';
						sendBuffer(reply2);
					}
			}
			return;
		}
		if (command.find(":") == std::string::npos)
		{
			cible = "412 :" + getNick() + " " + ":No text to send\n";
			sendBuffer(cible);
			return;
		}
		std::string message;
		message = command.substr(command.find(":"));
		std::stringstream buff;
		buff << command;
		buff >> cmd >> msg;
		cible = ":" + getNick() + " " + cmd + " " + target + " " + message + "\n";
		for (std::vector<Client*>::iterator it = _server->getClient().begin(); it != _server->getClient().end(); it++){
			if ((*it)->getNick() == target)
				(*it)->sendBuffer(cible);
		}
	}
}

void	Client::changeTopic(std::string command)
{
	std::string msg, channel, topic;
	std::stringstream buff;
	buff << command;
	buff >> channel >> msg;
	if (msg.size() > 0)
		msg = command.substr(command.find(":"));
	std::cout << "Get topic request from client " << getFd() << " : " << command << "fin" << std::endl;
	std::vector<Channel*>::iterator it = _chan.begin();
	while (it != _chan.end())
	{
		if ((*it)->getName() == channel)
			break;
		it++;
	}
	if (it == _chan.end())
		sendBuffer(ERR_NOTONCHANNEL(getPrefix(), channel));
	else if(!(*it)->isAdmin(this) && msg.size() > 0)
		sendBuffer(ERR_CHANOPRIVSNEEDED(getPrefix(), channel));
	else if((*it)->isAdmin(this) && msg.size() > 0)
	{
		msg = msg.substr(msg.find_first_not_of(": "));
		if (msg.size() == 1)
			msg = "";
		else
			std::remove((msg.begin() + msg.find_last_not_of(" \n")), msg.end(), ' ');
		(*it)->setTopic(msg);
		if ((*it)->getTopic().empty() || (*it)->getTopic().size() == 1)
			(*it)->broadcast(RPL_NOTOPIC(getPrefix(), channel));		
		else
			(*it)->broadcast(RPL_TOPIC(getPrefix(), channel, msg));
	}
	else if ((*it)->getTopic().empty() || (*it)->getTopic().size() == 1)
		sendBuffer(RPL_NOTOPIC(getPrefix(), channel));		
	else
		sendBuffer(RPL_TOPIC(getPrefix(), channel, (*it)->getTopic()));	
}

void	Client::sendBuffer(std::string buffer)
{
	_send.append(buffer);
}

void	Client::resetSend()
{
	_send.clear();
}

void	Client::deleteChan(Channel *channel)
{
	for (unsigned i = 0; i < _chan.size(); i++)
	{
		std::vector<Channel*>::iterator	it = _chan.begin() + i;
		if (channel == (*it))
		{
			_chan.erase(_chan.begin() + i);
			i = 0;
		}
	}
}
