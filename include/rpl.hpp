#pragma once

#include <iostream>



#define RPL_ENDOFWHO(client, mask)                                                              "315 " + client + " " + mask + " :End of WHO list\n"
#define RPL_WHOREPLY(client, channel, username, hostname, serverhostname, nickname, realname)   "352 " + client + ": " + channel + " " + username + " " + hostname + " " + serverhostname + " " + nickname + " H :0 " + realname + "\n"
#define RPL_WELCOME(client, text)	        "001 " + client + " :Welcome to the Internet Relay Network, " + text + "\n"
#define RPL_YOURHOST(client)		        "002 " + client + " :Your host is EasyRC, running version v0.1\n"
#define RPL_CREATED(client, date)	        "003 " + client + " :This server was created " + date
#define RPL_MYINFO(client)		            "004 " + client + " EasyRC v0.1 N/A N/A\n"
#define ERR_NEEDMOREPARAMS(client, command) "461 " + client + " " + command + ":Not enough parameters\n"
#define ERR_PASSWDMISMATCH(client)          "464 " + client + " :Password incorrect\n"
#define RPL_TOPIC(client, channel, topic)	"332" + client + " " + channel + " :" + topic + "\n"
#define RPL_NOTOPIC(client, channel)					"331 " + client + " " + channel + " :No topic is set\n"
#define RPL_NAMREPLY(client, channel, users)			"353 " + client + " = " + channel + " :" + users + "\n"
#define RPL_ENDOFNAMES(client, channel)					"366 " + client + " " + channel + " :End of /NAMES list.\n"

