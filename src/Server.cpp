#include "../include/Server.hpp"

//SERVER LAUNCHING//


Server::Server(std::string port, std::string password): _port(port), _password(password),  _clients(0), _clientsFd(NULL) 
{
	setTime();
	std::cout << "Server active" << std::endl;
	try {checkInput();}
	catch (Error::portNonDigit& error)
		{std::cerr << error.what() << std::endl;}
	catch (Error::portTooHigh& error)
		{std::cerr << error.what() << std::endl;}
	listening();
}

void Server::setTime()
{
    std::time_t result = std::time(NULL);
    struct tm* timeinfo;

    timeinfo = localtime(&result);
    _date = asctime(timeinfo);
}

void		Server::checkInput()
{
	for (int i = 0; _port[i] != 0; i++)
		if (!std::isdigit(_port[i]))
			throw Error::portNonDigit();
	if (std::atol(_port.c_str()) > 64738 || _port.size() > 5)
		throw Error::portTooHigh();
}


Server::~Server(){
	std::cout << "Server dead\n";}

//SERVER LISTENING//

void	Server::listening()
{
	struct sockaddr_in address;
	struct in_addr addr;
	addr.s_addr = INADDR_ANY;

	_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	int opt = 1;
	setsockopt(this->_sockfd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));
	fcntl(_sockfd, F_SETFL, O_NONBLOCK);
	address.sin_family = AF_INET;
	address.sin_addr = addr;
	address.sin_port = htons(std::atoi(_port.c_str()));

	bind(_sockfd, (struct sockaddr*)&address, sizeof(address));
	listen(_sockfd, 8);
	std::cout << "Waiting for connection...\n\n";
	createFd();
	while (1)
		waitInput();
}
//DATA REPLYING//

void	Server::whoReply(Client* client, char* buffer)
{
	buffer += 4;
	if(buffer[0] == '#')
		replyChannel(client, buffer);
	else
		replyUser(client, buffer);
}

void	Server::replyChannel(Client* client, char* buffer)
{
	buffer[strlen(buffer) - 1] = '\0';
	std::cout << "Get WHO request from client " << client->getFd() << " requesting info on " << buffer << std::endl;
	std::string	message;
	for(std::map<std::string,Channel*>::iterator it = _chanMap.begin(); it != _chanMap.end(); it++)
	{
		if((*it).second->getName().find(buffer) != std::string::npos)
		{
			for(std::vector<Client*>::iterator itt = it->second->getClient().begin(); itt != it->second->getClient().end(); itt++)
			{
				message = RPL_WHOREPLY(client->getHostname(), (*it).second->getName(), (*itt)->getUser(), (*itt)->getHostname(), "EasyRC.gg", (*itt)->getNick(), (*itt)->getFullName());
				std::cout << "Responding to client " << client->getFd() << " with message " << message;
				send(client->getFd(), message.c_str(), message.size(), 0);
			}
		}
	}
	message = RPL_ENDOFWHO(client->getHostname(), buffer);
	std::cout << "Responding to client " << client->getFd() << " with message " << message;
	send(client->getFd(), message.c_str(), message.size(), 0);
}

void	Server::replyUser(Client* client, char* buffer)
{
	buffer[strlen(buffer) - 1] = '\0';
	std::cout << "Get WHO request from client " << client->getFd() << " requesting info on " << buffer << std::endl;
	std::string	message;
	for(std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end();it++)
	{
		if ((*it)->getNick().find(buffer) != std::string::npos)
		{
			message = RPL_WHOREPLY(client->getHostname(), (*it)->getFirstChannel(), (*it)->getUser(), (*it)->getHostname(), "EasyRC.gg", (*it)->getNick(), (*it)->getFullName());
			std::cout << "Responding to client " << client->getFd() << " with message " << message;
			send(client->getFd(), message.c_str(), message.size(), 0);
		}
	}
	message = RPL_ENDOFWHO(client->getHostname(), buffer);
	std::cout << "Responding to client " << client->getFd() << " with message " << message;
	send(client->getFd(), message.c_str(), message.size(), 0);
}

void	Server::waitInput(){
	int val = poll(_clientsFd, _clients.size() + 1, -1);
	if (val < 0)
		std::cout << "Error poll\n";
	for (unsigned long i = 0; i < _clients.size() + 1; i++)
	{
		if (_clientsFd[i].revents != 0)
		{
			if (_clientsFd[i].fd == _sockfd){
				addClient();
				break;
			}
			else
				receiveData(this->_clients[i - 1]);
		}
	}
}

//NEW CLIENT HANDLING//

void	Server::createFd()
{
	if (this->_clientsFd)
		delete [] this->_clientsFd;
	this->_clientsFd = new struct pollfd[_clients.size() + 1];
	this->_clientsFd[0].fd = this->_sockfd;
	this->_clientsFd[0].events = POLLIN;
	for (unsigned long i = 0; i < this->_clients.size(); i++)
	{
		this->_clientsFd[i + 1].fd = this->_clients[i]->getFd();
		this->_clientsFd[i + 1].events = POLLIN;
	}
}


void	Server::addClient()
{
	int	socket = 0;
	while (socket != -1)
	{
		struct	sockaddr_in address;
		socklen_t addrlen = sizeof(address);
		socket = accept(_sockfd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
		std::string _ipClient = inet_ntoa(address.sin_addr);
		int	port = ntohs(address.sin_port);
		if (socket > 0)
		{
			Client* client = new Client(this, socket, _ipClient, port);
			_clients.push_back(client);
			createFd();
			fcntl(client->getFd(), F_SETFL, O_NONBLOCK);
			break;
		}
	}
}

void	Server::deleteClient(Client* client)
{
	std::vector<Client*>::iterator it = _clients.begin();
	int i = 0;
	while((*it)->getFd() != client->getFd())
	{
		it++;
		i++;
	}
	_clients.erase(_clients.begin() + i);
	close(client->getFd());
	delete client;
	createFd();
}

//DATA HANDLING//

void	Server::receiveData(Client *client){
	char	buffer[8192];
	int err = recv(client->getFd(), &buffer, sizeof(buffer), 0);
	buffer[err] = '\0';
	if (err == 0 && client->getCommand().size() == 0)
	{
		deleteClient(client);
		std::cout << "client disconnected\n";
	}
	else
		client->parseBuffer(buffer);
}

//CHANNEL CHECK//

void	Server::checkChannel(Client *client, std::string buffer){
	if (_chanMap.find(buffer) != _chanMap.end())
	{
		//Need to RPL to join chan + topic if any + client list
		//Need to add client to vector of channel
		_chanMap[buffer]->join(client);
		_chanMap[buffer]->update(client);
		//std::cout << buffer << "pipou\n";
	}
	else{
		_chanMap.insert(make_pair(buffer, new Channel(this, buffer, client)));
		//Need to send RPL_channel created for Konversation to create a chan
	}
}

void	Server::broadcast(std::string message)
{
	for(std::vector<Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
		send((*it)->getFd(), message.c_str(), message.size(), 0);
}
