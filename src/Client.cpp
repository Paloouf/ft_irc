#include "../include/Client.hpp"

Client::Client(Server *server, int fd, std::string hostname, int port) :_server(server), _fd(fd), _port(port),_hostname(hostname){
	std::cout << &_server << "Test client, fd: " << this->getFd() << ", hostname: " << this->getHostname() << std::flush;
	std::cout << ", port: " << this->getPort() << std::endl << std::endl;
	this->resetBuffer();	
	_negoCount = 0;
}

Client::~Client(){
	std::cout << "Client dead\n";
}

Client& Client::operator=(const Client& ref){
	_fd = ref.getFd();
	_hostname = ref.getHostname();
	_port = ref.getPort();
	return *this;
}

void	Client::addBuffer(char* buffer)
{
	_command.append(buffer);
}

void	Client::resetBuffer()
{
	_command.clear();
}

void	Client::parseBuffer(char * buffer)
{
	if (_negoCount < 4)
	{
		try
			{parseNego(buffer);}
		catch (Error::wrongPassword& error)
			{
				resetBuffer();
				_server->deleteClient(this);
				std::cerr << error.what() << std::endl;
			}
		catch (Error::wrongArgument& error)
			{
				resetBuffer();
				_server->deleteClient(this);
				std::cerr << error.what() << std::endl;
			}
		catch (Error::wrongNickname& error)
			{
				resetBuffer();
				_server->deleteClient(this);
				std::cerr << error.what() << std::endl;
			}
		catch (Error::usedNickname& error)
			{
				resetBuffer();
				_server->deleteClient(this);
				std::cerr << error.what() << std::endl;
			}
		catch (Error::usedUsername& error)
			{
				resetBuffer();
				_server->deleteClient(this);
				std::cerr << error.what() << std::endl;
			}	
	}
	else
		parseMsg(buffer);
}

//BEFORE NEGOTIATION//

void	Client::parseNego(char *buffer)
{
	std::string command = buffer;
	addBuffer(buffer);
	std::stringstream 	sBuff(getCommand());
	std::string			message;
	while (getline(sBuff, command))
	{
		std::cerr << "Negotiation step : Message from client " << getFd() << " : " << buffer;
		if (command.size() > 3 && command.substr(0,3) == "CAP" && getNego() == 0)
		{
			message = "CAP * LS :\n";
			send(getFd(), message.c_str(), message.size(), 0);
			std::cout << "Responding to client " << getFd() << " with message " << message;
			setNego(1);
		}
		else if (command.size() > 4 && command.substr(0,4) == "PASS" && getNego() == 1)
		{
			sBuff >> command;
			if (!command[0])
			{
				message = ERR_NEEDMOREPARAMS(getHostname(), "PASS");
				std::cout << "Responding to client " << getFd() << " with message " << message;
				send(getFd(), message.c_str(), message.size(), 0);
			}
			if (command != "PASS :" + _server->getPassword())
			{
				message = ERR_PASSWDMISMATCH(getHostname());
				std::cout << "Responding to client " << getFd() << " with message " << message ;
				send(getFd(), message.c_str(), message.size(), 0);
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
			sendWelcome();
			setNego(4);
		}
		else
			throw Error::wrongArgument();
	}
	resetBuffer();
}

void	Client::sendWelcome()
{
	std::string message = RPL_WELCOME(getNick(), getFullName());
	send(getFd(), message.c_str(), message.size(), 0);
	std::cout << "Responding to client " << getFd() << " with message " << message;
	message = RPL_YOURHOST(getNick());
	send(getFd(), message.c_str(), message.size(), 0);
	std::cout << "Responding to client " << getFd() << " with message " << message;
	message = RPL_CREATED(getNick(), _server->getDate());
	send(getFd(), message.c_str(), message.size(), 0);
	std::cout << "Responding to client " << getFd() << " with message " << message;
	message = RPL_MYINFO(getNick());
	send(getFd(), message.c_str(), message.size(), 0);
	std::cout << "Responding to client " << getFd() << " with message " << message;
	std::cout << "Successfully registered client " << getHostname() << std::endl << std::endl;
}

//AFTER NEGOTIATION//

void	Client::parseMsg(char *buffer)
{
	std::string command = buffer;
	std::cout << "MSG:" << command << std::endl;
	if (command.size() > 4 && command.substr(0,4) == "PING")
	{	
		std::cout << "Getting Ping request from client " << getFd() << std::endl;
		std::string pong = "PONG " + command.substr(5) + "\n";
		std::cout << "Responding to ping request from client " << getFd() << " with message " << pong << std::endl;
		send(getFd(), pong.c_str(), pong.size(), 0);
	}
	if (command.size() > 4 && command.substr(0,4) == "JOIN")
	{
		getServer()->checkChannel(this, command);
	}
	if (command.size() > 3 && command.substr(0,3) == "WHO")
		getServer()->whoReply(this, buffer);
	if (command.size() > 4 && command.substr(0,3) == "NICK")
	{
		if(checkNick(command.substr(5)) && checkDoubleNick(command.substr(5)))
			setNick(command.substr(5));
	}
	if (command.size() > 4 && command.substr(0,3) == "USER")
	{
		if(checkDoubleUser(command.substr(5).c_str()))
			setUser(command.substr(5));
	}
}

//GETTER//

std::string	Client::getFirstChannel() const
{
	if(_chan.empty())
		return ("*");
	return (_chan[0]->getName());
}

bool		Client::checkNick(std::string nick)
{
	std::string	message;
	if (nick[0] == '#' || nick[0] == '&' || nick[0] == ':' || nick[0] == '@' || nick[0] == '!' || nick.find(' ') != std::string::npos)
	{
		std::cout << nick << std::endl;
		message = ERR_ERRONEUSNICKNAME(getHostname(), nick);
		send(getFd(), message.c_str(), message.size(), 0);
		return false;
	}	
	return true;
}

bool		Client::checkDoubleNick(std::string nick)
{
	std::string	message;
	for (std::vector<Client*>::iterator it = getServer()->getClient().begin(); it != getServer()->getClient().end(); it++)
	{
		if((*it)->getNick() == nick)
		{
			message = ERR_NICKNAMEINUSE(getHostname(),nick);
			send(getFd(), message.c_str(), message.size(), 0);
			return false;
		}
	}
	return true;
}

bool		Client::checkDoubleUser(const char* user)
{
	std::string	message;
	for (std::vector<Client*>::iterator it = getServer()->getClient().begin(); it != getServer()->getClient().end(); it++)
	{
		if((*it)->getUser() == user)
		{				
			message = ERR_ALREADYREGISTERED(getHostname());
			send(getFd(), message.c_str(), message.size(), 0);
			return false;
		}
	}
	return true;
}