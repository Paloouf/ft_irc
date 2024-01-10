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
	std::string command = buffer;
	std::cout << command << std::endl;
	if (command.size() != 0)
		addBuffer(buffer);
	else
	{
		std::cout << "here" << std::endl;
		std::stringstream 	sBuff(getCommand());
		std::string			message;
		while (getline(sBuff, command))
		{
			std::string test;			
			sBuff >> test;
			{
				std::cout << test << std::endl;
				if (!test.compare("CAP"))
				{
					message = "CAP * LS :";
					send(getFd(), message.c_str(), message.size(), 0);
					std::cout << message << std::endl;
				}
				if (!test.compare("PASS"))
				{
					sBuff >> test;
					if (!test[0])
					{
						message = ERR_NEEDMOREPARAMS(getHostname(), "PASS");
						send(getFd(), message.c_str(), message.size(), 0);
					}
					if (test != ":" + _server->getPassword())
						std::cout << "ERROR: WRONG PASSWORD\n";
				}
				if (!test.compare("NICK"))
				{
					sBuff >> test;
					setNick(test);
				}
				if (!test.compare("USER"))
				{
					sBuff >> test;
					setUser(test);
				}
				if (!test.compare("PING"))
				{
					sBuff >> test;
					std::string pong = "PONG " + test + "\n";
					send(getFd(), pong.c_str(), pong.size(), 0);
				}
			}
		}
		resetBuffer();
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