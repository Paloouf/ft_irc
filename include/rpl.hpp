#pragma once

#include <iostream>


#define RPL_WELCOME(client, text)	        "001 " + client + " :Welcome to the Internet Relay Network, " + text
#define RPL_YOURHOST(client)		        "002 " + client + " :Your host is EasyRC, running version v0.1\n"
#define RPL_CREATED(client, date)	        "003 " + client + " :This server was created " + date
#define RPL_MYINFO(client)		            "004 " + client + " EasyRC v0.1 N/A N/A\n"
#define ERR_NEEDMOREPARAMS(client, command)  "461 " + client + " " + command + ":Not enough parameters"