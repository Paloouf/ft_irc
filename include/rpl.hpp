#pragma once

#include <iostream>

#define RPL_AWAY(client, nick, message)	                                                            "301 " + client + " " + nick + " :" + message
#define RPL_CREATED(client, date)	                                                                "003 " + client + " :This server was created " + date
#define RPL_ENDOFWHO(client, mask)                                                                  "315 " + client + " " + mask + " :End of WHO list\n"
#define RPL_ENDOFNAMES(client, channel)					                                            "366 " + client + " " + channel + " :End of /NAMES list.\n"
#define RPL_INVITING(prefix, nick, target)      ":" + prefix + " INVITE " + nick + " " + target + "\n"
#define RPL_JOIN(prefix, target)	            ":" + prefix + " JOIN " + target + "\n"
#define RPL_KICK(prefix, channel, target)       ":" + prefix + " KICK " + channel + " " + target + "\n"
#define RPL_MYINFO(client)		                                                                    "004 " + client + " EasyRC v0.1 N/A N/A\n"
#define RPL_NAMREPLY(client, channel, users)                                                    	"353 " + client + " = " + channel + " :" + users + "\n"
#define RPL_NOTOPIC(client, channel)			                                                    "331 " + client + " " + channel + " :No topic is set\n"
#define RPL_PART(prefix, target)	            ":" + prefix + " PART " + target + "\n"
#define RPL_TOPIC(client, channel, topic)	                                                        "332" + client + " " + channel + " :" + topic + "\n"
#define RPL_WELCOME(client, text)	                                                                "001 " + client + " :Welcome to the Internet Relay Network, " + text + "\n"
#define RPL_WHOREPLY(client, channel, username, hostname, serverhostname, nickname, realname)       "352 " + client + ":" + channel + " " + username + " " + hostname + " " + serverhostname + " " + nickname + " H :0 " + realname + "\n"
#define RPL_YOURHOST(client)		                                                                "002 " + client + " :Your host is EasyRC, running version v0.1\n"
#define RPL_CREATED(client, date)	                                                              "003 " + client + " :This server was created " + date
#define RPL_MYINFO(client)		                                                                  "004 " + client + " EasyRC v0.1 N/A N/A\n"
#define RPL_ENDOFWHO(client, mask)                                                              "315 " + client + " " + mask + " :End of WHO list\n"
#define RPL_NOTOPIC(client, channel)					                                                  "331 " + client + " " + channel + " :No topic is set\n"
#define RPL_TOPIC(client, channel, topic)	                                                      "332" + client + " " + channel + " :" + topic + "\n"
#define RPL_WHOREPLY(client, channel, username, hostname, serverhostname, nickname, realname)   "352 " + client + ":" + channel + " " + username + " " + hostname + " " + serverhostname + " " + nickname + " H :0 " + realname + "\n"
#define RPL_NAMREPLY(client, channel, users)			                                              "353 " + client + " = " + channel + " :" + users + "\n"
#define ERR_ERRONEUSNICKNAME(client, nick)                                                      "432 " + client + " " + nick + " :Erroneus nickname\n"
#define ERR_NOSUCHCHANNEL(client, channel)                                                      "403 " + client + " " + channel + " :No such channel\n"
#define ERR_NICKNAMEINUSE(client, nick)                                                             "433 " + client + " " + nick + " :Nickname is already in use\n"
#define ERR_NOTONCHANNEL(client, channel)                                                       "442 " + client + " " +  channel + " :You're not on that channel\n"
#define ERR_NEEDMOREPARAMS(client, command)                                                         "461 " + client + " " + command + ":Not enough parameters\n"
#define ERR_ALREADYREGISTERED(client)                                                               "462 " + client + " :You may not reregister\n"
#define ERR_PASSWDMISMATCH(client)                                                                  "464 " + client + " :Password incorrect\n"
#define ERR_CHANNELISFULL(client, channel)                                                          "471 " + client + " " + channel + " :Cannot join channel (+l)\n"
#define ERR_INVITEONLY(client, channel)                                                             "473 " + client + " " + channel + " :Cannot join channel (+i)\n"
#define NEW_NICK(old, newnick)                  ":" + old + " NICK " + newnick + "\n"

//QUIT
#define QUIT(prefix)                        ":" + prefix + " QUIT : Quit: Bye for now!\r\n"

//MODES
#define RPL_ADDOP(prefix, target, msg)		    ":" + prefix + " MODE " + target + " +o " + msg + "\n"
#define RPL_REMOP(prefix, target, msg)   	    ":" + prefix + " MODE " + target + " -o " + msg + "\n"
#define RPL_ADDPASS(prefix, target, msg)	    ":" + prefix + " MODE " + target + " +k " + msg + "\n"
#define RPL_REMPASS(prefix, target)	            ":" + prefix + " MODE " + target + " -k\n"
#define RPL_ADDLIMIT(prefix, target, limit)     ":" + prefix + " MODE " + target + " +l " + limit + "\n" 
#define RPL_REMLIM(prefix, target)			    ":" + prefix + " MODE " + target + " -l\n"
#define RPL_ADDINV(prefix, target)              ":EasyRC.gg MODE " + target + " +i\n"
#define RPL_REMINV(prefix, target)			    ":EasyRC.gg MODE " + target + " -i\n"
#define RPL_ADDTOP(prefix, target)			    ":" + prefix + " MODE " + target + " +t\n"
#define RPL_REMTOP(prefix, target)			    ":" + prefix + " MODE " + target + " -t\n"

//PING
#define PONG(buffer)                        "PONG " + buffer + "\n"

