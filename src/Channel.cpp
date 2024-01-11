#include "../include/main.hpp"

Channel::Channel(Server *server, Client *autor, std::string name) :_server(server), _name(name), _creator(autor){}
Channel::~Channel(){}
