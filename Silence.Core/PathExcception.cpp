
#include "PathException.h"

PathException::PathException(std::string msg)
    : message(msg)
{
}

PathException::~PathException() throw()
{
}

const char * PathException::what() const throw()
{
    return message.c_str();
}