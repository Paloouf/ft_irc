#include "../include/Server.hpp"

//SERVER LAUNCHING//

Server::Server(std::string port, std::string password): _port(port), _password(password)
{
	setTime();
	std::cout << "Server active" << std::endl;
	try {checkInput();}
	catch (Server::portNonDigit& error)
		{std::cerr << error.what() << std::endl;}
	catch (Server::portTooHigh& error)
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
			throw Server::portNonDigit();
	if (std::atol(_port.c_str()) > 64738 || _port.size() > 5)
		throw Server::portTooHigh();
}

Server::~Server(){
	std::cout << "Server dead\n";}

//DATA RECEIVING//

void	Server::listening(){
	_chan.push_back(new Channel(this, "&General"));
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
	std::cout << "Waiting for connection...\n";
	createFd();
	while (1)
		waitInput();
}

void	Server::waitInput(){
	int val = poll(_clientsFd, _clients.size() + 1, -1);
	//std::cout << _clients.size() << std::endl;
	if (val < 0)
		std::cout << "error poll\n";
	for (unsigned long i = 0; i < _clients.size() + 1; i++)
	{
		std::cout << "i dans le for: " << i << std::endl;
		if (_clientsFd[i].revents != 0)
		{
			if (_clientsFd[i].fd == _sockfd)
			{
				addClient();
				std::cout << "bomboclat\n";
			}
			else
			{
				std::cout << "c'est le else if\n";
				Client *client = this->_clients[i - 1];
				receiveData(client);
			}
		}

	}
}

//NEW CLIENT HANDLING//

void	Server::createFd(){
	if (this->_clientsFd)
		delete [] this->_clientsFd;
	this->_clientsFd = new struct pollfd[_clients.size() + 1];

	this->_clientsFd[0].fd = this->_sockfd;
	this->_clientsFd[0].events = POLLIN;

	for (unsigned long i = 0; i < this->_clients.size(); i++)
	{
		this->_clientsFd[i + 1].fd = this->_clients[i]->getFd();
		this->_clientsFd[i + 1].events = POLLIN;
		std::cout << "clientFd finito\n";
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
			parseBuffer(client);
			break;
		}
	}
}

//DATA HANDLING//

void	Server::receiveData(Client *client){
	char	buffer[8192];
	int err = recv(client->getFd(), &buffer, sizeof(buffer), 0);
	if (err == 0)
	{
		for(unsigned long i = 0; i < _clients.size(); i++)
		{
			if (_clients[i]->getFd() == client->getFd())
				_clients.erase(_clients.begin() + i);
		}
		std::cout << "client disconnected\n";
	}
	else
	{
		std::stringstream sBuff(buffer);
		std::string str;
		sBuff >> str;
		if (!str.compare("PING"))
		{
			sBuff >> str;
			std::string pong = "PONG " + str + "\n";
			send(client->getFd(), pong.c_str(), pong.size(), 0);
			std::cout << pong ;
		}
		buffer[err] = '\0';
		std::string buff = buffer;
		std::cout << buff << std::endl;
	}
}

void	Server::parseBuffer(Client* client)
{
	char	buffer[8192];
	std::string extract;
	int err = recv(client->getFd(), &buffer, sizeof(buffer), 0);
	fcntl(client->getFd(), F_SETFL, O_NONBLOCK);
	std::string test;
	while (err != -1)
	{
		std::cout << buffer << std::endl;
		std::stringstream sBuff(buffer);
		while(sBuff >> test)
		{
			if (!test.compare("PASS"))
			{
				sBuff >> test;
				if (test != getPassword())
					std::cout << "ERROR: WRONG PASSWORD\n";
				//std::cout << client->getNick() << std::endl;
			}
			if (!test.compare("NICK"))
			{
				sBuff >> test;
				std::cout << "NIQUE: " <<  test << std::endl;
				client->setNick(test);
			}
			if (!test.compare("USER"))
			{
				sBuff >> test;
				client->setUser(test);
			}
			extract = buffer;
			if (extract.find(":") != std::string::npos)
			{
				extract = buffer + extract.find(":") + 1;
				client->setFullName(extract);
			}
		}
		std::cout << err << std::endl;
		err = recv(client->getFd(), &buffer, sizeof(buffer), 0);

		std::cout << err << test << std::endl;
	}
	std::string welcome = RPL_WELCOME(client->getNick(), client->getFullName());
	send(client->getFd(), welcome.c_str(), welcome.size(), 0);
	welcome = RPL_YOURHOST(client->getNick());
	send(client->getFd(), welcome.c_str(), welcome.size(), 0);
	welcome = RPL_CREATED(client->getNick(), this->_date);
	send(client->getFd(), welcome.c_str(), welcome.size(), 0);
	welcome = RPL_MYINFO(client->getNick());
	send(client->getFd(), welcome.c_str(), welcome.size(), 0);
	std::cout << "nique: " << client->getNick() << std::endl;
}



