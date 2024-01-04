#include "../include/main.hpp"

Channel::Channel(Server *server, std::string name) :_server(server), _name(name){}
Channel::~Channel(){}
