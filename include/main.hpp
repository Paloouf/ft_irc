#pragma once

#include <iostream>
#include <stdexcept>
#include <stdint.h>
#include <vector>
#include <netinet/in.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include "Client.hpp"
#include "Server.hpp"
#include "Channel.hpp"
#include "rpl.hpp"
#include <poll.h>
#include <cstdlib>
#include <sstream>