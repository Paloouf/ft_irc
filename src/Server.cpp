#include "../include/Server.hpp"

void		Server::checkInput()
{
	for (int i = 0; _port[i] != 0; i++)
		if (!std::isdigit(_port[i]))
			throw Server::portNonDigit();
	if (std::atol(_port.c_str()) > 49151 || _port.size() > 5)
		throw Server::portTooHigh();
}

Server::~Server(){
	std::cout << "Server dead\n";}

Server::Server(std::string port, std::string password): _port(port), _password(password)
{
	std::cout << "Server active" << std::endl;
	try {checkInput();}
	catch (Server::portNonDigit& error)
		{std::cerr << error.what() << std::endl;}
	catch (Server::portTooHigh& error)
		{std::cerr << error.what() << std::endl;}
}

std::string	Server::getPassword(){
	return (_password);}

std::string Server::getPort(){
	return (_port);}

