#pragma once

#include <iostream>

#define RPL_WELCOME(client, text)																	"001 " + client + " :Welcome to the Internet Relay Network, " + text + "\n"
#define RPL_YOURHOST(client)																		"002 " + client + " :Your host is EasyRC, running version v0.1\n"
#define RPL_CREATED(client, date)																	"003 " + client + " :This server was created " + date
#define RPL_MYINFO(client)																			"004 " + client + " EasyRC v0.1 N/A N/A\n"
#define RPL_AWAY(client, nick, message)																"301 " + client + " " + nick + " :" + message + "\n"
#define RPL_ENDOFWHO(client, mask)																	"315 " + client + " " + mask + " :End of WHO list\n"
#define RPL_NOTOPIC(client, channel)																"331 " + client + " " + channel + " :No topic is set\n"
#define RPL_TOPIC(client, channel, topic)															"332 " + client + " " + channel + " :" + topic + "\n"
#define RPL_WHOREPLY(client, channel, username, hostname, serverhostname, nickname, realname)		"352 " + client + ":" + channel + " " + username + " " + hostname + " " + serverhostname + " " + nickname + " H :0 " + realname + "\n"
#define RPL_NAMREPLY(client, channel, users)														"353 " + client + " = " + channel + " :" + users + "\n"
#define RPL_ENDOFNAMES(client, channel)																"366 " + client + " " + channel + " :End of /NAMES list.\n"
#define ERR_NOSUCHCHANNEL(client, channel)															"403 " + client + " " + channel + " :No such channel\n"
#define ERR_ERRONEUSNICKNAME(client, nick)															"432 " + client + " " + nick + " :Erroneus nickname\n"
#define ERR_NICKNAMEINUSE(client, nick)																"433 " + client + " " + nick + " :Nickname is already in use\n"
#define ERR_NOTONCHANNEL(client, channel)															"442 " + client + " " +  channel + " :You're not on that channel\n"
#define ERR_NEEDMOREPARAMS(client, command)															"461 " + client + " " + command + ":Not enough parameters\n"
#define ERR_ALREADYREGISTERED(client)																"462 " + client + " :You may not reregister\n"
#define ERR_PASSWDMISMATCH(client)																	"464 " + client + " :Password incorrect\n"
#define ERR_CHANNELISFULL(client, channel)															"471 " + client + " " + channel + " :Cannot join channel (+l)\n"
#define ERR_INVITEONLY(client, channel)																"473 " + client + " " + channel + " :Cannot join channel (+i)\n"
#define ERR_BADCHANNELKEY(client, channel)															"475 " + client + " " + channel + " :Cannot join channel (+k)\n"
#define ERR_CHANOPRIVSNEEDED(client, channel)														"482 " + client + " " + channel + " :You're not channel operator\n"
#define RPL_PART(prefix, target)																	":" + prefix + " PART " + target + "\n"
#define RPL_KICK(prefix, channel, target)															":" + prefix + " KICK " + channel + " " + target + "\n"
#define RPL_JOIN(prefix, target)																	":" + prefix + " JOIN " + target + "\n"
#define RPL_INVITING(prefix, nick, target)															":" + prefix + " INVITE " + nick + " " + target + "\n"
#define NEW_NICK(old, newnick)																		":" + old + " NICK " + newnick + "\n"
#define SEND_PRIVMSG(client, target, msg)															":" + client + " PRIVMSG " + target + " :" + msg + "\n"
#define SEND_MODE(client, target, mode, msg)														":" + client + " MODE " + target + " " + mode + " " + msg + "\n"

//QUIT
#define QUIT(client)																				":" + client + " QUIT :Quit: Bye for now!\r\n"

//MODES
#define RPL_ADDOP(client, target, msg)																":" + client + " MODE " + target + " +o " + msg + "\n"
#define RPL_REMOP(client, target, msg)																":" + client + " MODE " + target + " -o " + msg + "\n"
#define RPL_ADDPASS(client, target, msg)															":" + client + " MODE " + target + " +k " + msg + "\n"
#define RPL_REMPASS(client, target)																	":" + client + " MODE " + target + " -k\n"
#define RPL_ADDLIMIT(client, target, limit)															":" + client + " MODE " + target + " +l " + limit + "\n" 
#define RPL_REMLIM(client, target)																	":" + client + " MODE " + target + " -l\n"
#define RPL_ADDINV(client, target)																	":EasyRC.gg MODE " + target + " +i\n"
#define RPL_REMINV(client, target)																	":EasyRC.gg MODE " + target + " -i\n"
#define RPL_ADDTOP(client, target)																	":" + client + " MODE " + target + " +t\n"
#define RPL_REMTOP(client, target)																	":" + client + " MODE " + target + " -t\n"

//PING
#define PONG(buffer)																				"PONG " + buffer + "\n"

