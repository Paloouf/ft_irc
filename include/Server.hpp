#pragma once

#include "main.hpp"

class Server{
	private:
		Server(){};
		std::string _port;
		std::string _password;
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
					return ("Port must be below 49151");
				}
		};
};