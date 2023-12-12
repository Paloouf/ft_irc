/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltressen <ltressen@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 10:55:27 by ltressen          #+#    #+#             */
/*   Updated: 2023/12/12 16:06:12 by ltressen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Client.hpp"

Client::Client(Server *server, int fd, std::string hostname, int port) :_fd(fd), _hostname(hostname), _port(port){
	std::cout << "Test client, fd: " << this->getFd() << ", hostname: " << this->getHostname() << std::flush;
	std::cout << ", port: " << this->getPort() << std::endl;	
}

Client::~Client(){
	std::cout << "Client dead\n";
}
