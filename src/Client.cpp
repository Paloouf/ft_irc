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
		catch (Server::wrongPassword& error)
			{
				resetBuffer();
				_server->deleteClient(this);
				std::cerr << error.what() << std::endl;
			}
		catch (Server::wrongArgument& error)
			{
				resetBuffer();
				_server->deleteClient(this);
				std::cerr << error.what() << std::endl;
			}
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
				throw Server::wrongPassword();
			}
			setNego(2);
		}
		else if (command.size() > 4 && command.substr(0,4) == "NICK" && getNego() == 2) 
		{
			setNick(command.substr(5));
			setNego(3);
		}
		else if (command.size() > 4 && command.substr(0,4) == "USER" && getNego() == 3)
		{
			char* commandbis = &command[5];
			setUser(strtok(commandbis, " "));
			std::cout << getUser() << " poueeeeeeeeeeeettt\n";
			setFullName(command.substr(command.find(":") + 1));
			sendWelcome();
			setNego(4);
		}
		else
			throw Server::wrongArgument();
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
	if (command.size() > 4 && command.substr(0,4) == "JOIN")
	{
		getServer()->checkChannel(this, command.substr(5, command.size() - 6));
	}
	if (command.substr(0,7) == "PRIVMSG")
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
	}
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