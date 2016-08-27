#include "GPU.h"

GPU::GPU(GLboolean print)
    : grabbedGPU(false)
{
    if (glewInit() == GLEW_OK) 
    {
        shaderVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
        renderer = glGetString(GL_RENDERER);
        version = glGetString(GL_VERSION);
        vendor = glGetString(GL_VENDOR);

        if (print) 
        {
            std::cout << "Shader Version : " << shaderVersion << std::endl;
            std::cout << "Renderer : " << renderer << std::endl;
            std::cout << "Version : " << version << std::endl;
            std::cout << "Vendor : " << vendor << std::endl;
        }

        grabbedGPU = true;
    }
    else 
    {
        std::cout << "Error grabbing GPU" << std::endl;
    }
}

GPU::~GPU()
{
}

GLboolean GPU::has(GRAPHIC_API type, GLdouble version) 
{
    if (type == OPENGL)
    {
        auto minVersion = static_cast<GLfloat>(version);
        auto f = 0.0f, s = 0.0f;
        
        glGetFloatv(GL_MAJOR_VERSION, &f);
        glGetFloatv(GL_MINOR_VERSION, &s);
        
        return(f + s >= version);
    }

    return false;
}