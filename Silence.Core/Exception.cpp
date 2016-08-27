
#include "Exception.h"

Error::Error(Component sys, std::string msg, Author user) :
    errorMessage(msg),
    system(sys),
    owner(user)
{
}

Error::~Error() throw()
{
}

const char * Error::what() const
{
    static std::string message = "Error thrown by ";

    switch (owner)
    {
        case Author::William: message += " William Taylor\n\nMessage ->";
            break;
        default: 
            break;
    }

    message.append(errorMessage.c_str());
    return message.c_str();
}

const char * Error::title() const
{
    static std::string title = "Error !!!";

    switch (system)
    {
        case Component::Network: title = "Networking error";
            break;
        case Component::Renderer: title = "Renderer error";
            break;
        case Component::Texture: title = "Texture error";
            break;
        case Component::FileIO: title = "File_IO error";
            break;
        case Component::Window: title = "Window error";
            break;
        case Component::Audio: title = "Audio error";
            break;
        case Component::Label: title = "Label error";
            break;
        case Component::Game: title = "Game error";
            break;
        default:
            break;
    }

    return title.c_str();
}
