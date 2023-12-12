/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltressen <ltressen@student.42perpignan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 10:29:08 by ltressen          #+#    #+#             */
/*   Updated: 2023/12/12 16:16:43 by ltressen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/main.hpp"

static bool	isNotNumber(char* line)
{
	for (int i = 0; line[i] != 0; i++)
		if (std::isdigit(line[i]))
			return (false);
	return (true);
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Usage is : <port number> <password>" << std::endl;
		return (1);
	}
	if (isNotNumber(argv[1]))
	{
		std::cout << "Port must only contain numbers" << ::std::endl;
		return (2);
	}
	Server	server(argv[1], argv[2]);
	return (0);
}