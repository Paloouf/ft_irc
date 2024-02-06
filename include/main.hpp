#pragma once

#include <algorithm>
#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fcntl.h>
#include <iterator>
#include <iomanip>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <poll.h>
#include <sstream>
#include <stdexcept>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <cerrno>
#include "Error.hpp"
#include "Channel.hpp"
#include "Client.hpp"
#include "rpl.hpp"
#include "Server.hpp"