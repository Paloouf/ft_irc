#pragma once

#include "main.hpp"
class Error
{
    public:
		class	portNonDigit : public std::exception
		{
			public:
				virtual const char* what() const throw()
				{return ("Port must be a only digits arguments");}
		};
		class	portTooHigh : public std::exception
		{
			public:
				virtual const char* what() const throw()
				{return ("Port must be below 64738");}
		};
		class	wrongPassword : public std::exception
		{
			public:
				virtual const char* what() const throw()
				{return ("Password doesn't match, deleting the client");}
		};
		class	missingArgument : public std::exception
		{
			public:
				virtual const char* what() const throw()
				{return ("Missing Arguments, deleting the client");}
		};
		class	wrongArgument : public std::exception
		{
			public:
				virtual const char* what() const throw()
				{return ("Argument doesn't match negotiation step");}
		};
        class   wrongNickname : public std::exception
        {
			public:
				virtual const char* what() const throw()
				{return ("Error in nickname");}
        };
        class   usedNickname : public std::exception
        {
			public:
				virtual const char* what() const throw()
				{return ("Nickname already used");}
        };
        class   usedUsername : public std::exception
        {
			public:
				virtual const char* what() const throw()
				{return ("Username already used");}
        };
};