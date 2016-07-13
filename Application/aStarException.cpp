
#include "aStarException.h"

A_StarException::A_StarException(std::string msg)
	: message(msg)
{
}

A_StarException::~A_StarException() throw()
{
}

const char * A_StarException::what() const throw()
{
	return message.c_str();
}