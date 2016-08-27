
#pragma once

#include <exception>
#include <string>

#if defined DLL_EXPORT 
    #define SILENCE_EXPORT __declspec(dllexport)
#else
    #define SILENCE_EXPORT __declspec(dllimport)
#endif

#pragma warning(disable: 4290)
#pragma warning(disable: 4275)
#pragma warning(disable: 4251)

enum class Component { Label, Texture, Network, Game, Window, Renderer,  Audio,  FileIO, System };
enum class Author { William };

class SILENCE_EXPORT Error : public std::exception
{
    std::string errorMessage;
    Component system;
    Author owner;
public:
    Error(Component sys, std::string msg, Author user = Author::William);
    virtual ~Error() throw();
    
    const char * title() const;
    const char * what() const override;
};