#include "../include/Client.hpp"

Client::Client(Server *server, int fd, std::string hostname, int port) :_server(server), _fd(fd), _port(port),_hostname(hostname){
	std::cout << &_server << "Test client, fd: " << this->getFd() << ", hostname: " << this->getHostname() << std::flush;
	std::cout << ", port: " << this->getPort() << std::endl;
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
	if (_negoCount < 5)
		parseNego(buffer);
	else
		parseMsg(buffer);
}

void	Client::parseNego(char *buffer)
{
	std::string command = buffer;
	std::cout << command << std::endl;
	addBuffer(buffer);
	std::cout << "here" << std::endl;
	std::stringstream 	sBuff(getCommand());
	std::string			message;
	while (getline(sBuff, command))
	{
		std::cerr << "Negotiation step : Message from client " << getFd() << " : <" << buffer << ">" <<std::endl;
		if (command.size() > 3 && command.substr(0,3) == "CAP" && getNego() == 0)
		{
			message = "CAP * LS :\n";
			send(getFd(), message.c_str(), message.size(), 0);
			std::cout << "Responding with message <" << message << "> to client " << getFd() << std::endl << std::endl;
			setNego(1);
		}
		else if (command.size() > 4 && command.substr(0,4) == "PASS" && getNego() == 1)
		{
			sBuff >> command;
			if (!command[0])
			{
				message = ERR_NEEDMOREPARAMS(getHostname(), "PASS");
				std::cout << "Responding with message <" << message << "> to client " << getFd() << std::endl << std::endl;
				send(getFd(), message.c_str(), message.size(), 0);
			}
			if (command != "PASS :" + _server->getPassword())
			{
				message = ERR_PASSWDMISMATCH(getHostname());
				std::cout << "Responding with message <" << message << "> to client " << getFd() << std::endl << std::endl;
				send(getFd(), message.c_str(), message.size(), 0);
				throw Server::wrongPassword();
			}
			std::cout << "here" << std::endl;
			setNego(2);
		}
		else if (command.substr(0,4) == "NICK")
		{
			setNick(command.substr(5));
		}
		if (command.substr(0,4) == "USER")
		{
			setUser(command.substr(5));
			setFullName(command.substr(command.find(":") + 1));
			sendWelcome();
			_negoCount = 5;
		}


	}
	resetBuffer();
}

void	Client::parseMsg(char *buffer)
{
	std::string command = buffer;
	std::cout << "MSG:" << command << std::endl;
	if (command.substr(0,4) == "PING")
	{
		std::string pong = "PONG " + command.substr(5) + "\n";
		std::cout << pong;
		send(getFd(), pong.c_str(), pong.size(), 0);
	}
	if (command.size() > 6 && command.substr(0,4) == "JOIN" && command.substr(5,6) == " #")
	{
		std::string nameChan = command.substr(command.find("#") + 1);
		std::map<std::string, Channel*>::iterator		it = _server->getChanMap()->find(nameChan);
		if (it != _server->getChanMap()->end())
		{
			_chan.push_back(it->second);
		}
		else
		{
			_chan.push_back(_server->createChan(nameChan, this));
			std::cout << "channel: " << nameChan << " was created by " << getFullName() << " at " << _server->getDate() << std::endl;
		}
	}
}


void	Client::sendWelcome()
{
	std::string message = RPL_WELCOME(getNick(), getFullName());
	send(getFd(), message.c_str(), message.size(), 0);
	message = RPL_YOURHOST(getNick());
	send(getFd(), message.c_str(), message.size(), 0);
	message = RPL_CREATED(getNick(), _server->getDate());
	send(getFd(), message.c_str(), message.size(), 0);
	message = RPL_MYINFO(getNick());
	send(getFd(), message.c_str(), message.size(), 0);
	std::cout << "nique: " << getNick() << std::endl;
}
