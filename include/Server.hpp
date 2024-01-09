#pragma once

#include "main.hpp"
class Client;
class Channel;

class Server{
	private:
		Server(){};
		std::string _port;
		std::string _password;
		std::string _date;
		std::vector<Client*> _clients;
		std::vector<Channel*>	_chan;
		struct pollfd	*_clientsFd;
		int	_sockfd;
		void	parseBuffer(Client* client);
		void	sendWelcome(Client* client);
		void	listening();
		void	createFd();
		void	waitInput();
		void	addClient();
		void	setTime();
		void	receiveData(Client *client);
		std::string	convertIP(const void *address);

		//void	createClient();
	public:
		~Server();
		Server(std::string port, std::string password);
		void		checkInput();
		std::string getPassword();
		std::string getPort();
		

		class	portNonDigit : public std::exception
		{
			public:
				virtual const char* what() const throw()
				{
					return ("Port must be a only digits arguments");
				}
		};
		class	portTooHigh : public std::exception
		{
			public:
				virtual const char* what() const throw()
				{
					return ("Port must be below 64738");
				}
		};
		class	wrongPassword : public std::exception
		{
			public:
				virtual const char* what() const throw()
				{
					return ("Password doesn't match, deleting the client");
				}
		};
		class	missingArgument : public std::exception
		{
			virtual const char* what() const throw()
				{
					return ("Port must be below 64738");
				}
		};
};
