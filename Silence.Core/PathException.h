
#pragma once

#include "EngineLayer.h"

class SILENCE_EXPORT PathException : public std::exception
{
    std::string message;
public:
    explicit PathException(std::string);
    ~PathException() throw();

    const char * what() const throw() override;
};