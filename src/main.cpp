#include "../include/main.hpp"


int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Usage is : ./ircsev <port number> <password>" << std::endl;
		return (1);
	}
	Server	server(argv[1], argv[2]);
	return (0);
}

