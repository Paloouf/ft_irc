/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltressen <ltressen@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 10:57:18 by ltressen          #+#    #+#             */
/*   Updated: 2023/12/12 16:06:46 by ltressen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "main.hpp"
class Server;

class Client{
	public:
		Client(Server* server, int fd, std::string hostname, int port);
		~Client();

		//GETTERS
		int	getFd() const{return _fd;}
		std::string const &	getHostname() const{return _hostname;}
		int	getPort() const {return _port;}
	private:
		int	_fd;
		std::string _hostname;
		int	_port;	
		Server* server;
};