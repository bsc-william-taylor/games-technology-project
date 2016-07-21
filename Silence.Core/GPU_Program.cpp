
/**
*
* Copyright (c) 2014 : William Taylor : wi11berto@yahoo.co.uk
*
* This software is provided 'as-is', without any
* express or implied warranty. In no event will
* the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute
* it freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented;
*    you must not claim that you wrote the original software.
*    If you use this software in a product, an acknowledgment
*    in the product documentation would be appreciated but
*    is not required.
*
* 2. Altered source versions must be plainly marked as such,
*    and must not be misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source distribution.
*/

#include "GPU_Program.h"

// Constructor & Deconstructor 
GPU_Program::GPU_Program()
    : compiled(GL_TRUE)
{
    // first create a program object
    programID = glCreateProgram();
    // then the vertex and fragment shaders
    fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
    vertexID = glCreateShader(GL_VERTEX_SHADER);
}

GPU_Program::~GPU_Program()
{
    // then delete the program and other sub program when the object is deleted
    glDeleteProgram(programID);
    glDeleteShader(fragmentID);
    glDeleteShader(vertexID);
}

// Here we just set the fragment source
void GPU_Program::setFragmentSource(const char * source)
{
    //  if the source is not a filename
    if (std::string(source).find(".glsl") == std::string::npos)
    {
        // we just load it directly into the shader
        glShaderSourceARB(fragmentID, 1, &source, NULL);
        glCompileShader(fragmentID);
    }
    else
    {
        // else open up the file and load it as the shader source
        std::ifstream file(source);
        if (file.is_open())
        {
            // set first line
            std::string src = SHADER_VERSION;
            // then loop through each line adding it to the source
            while (!file.eof())
            {
                std::string line = "";
                std::getline(file, line);
                src += line;
            }
    
            // then grab the full source from the string
            const GLchar * c_source = src.c_str();

            // and send it to the shader
            glShaderSourceARB(fragmentID, 1, &c_source, NULL);
            glCompileShader(fragmentID);
        }
        else
        {
            // print error when we cant open the file
            std::cout << "Error opening file " << source << std::endl;
        }
    }

    // then here we always check to make sure it compiled successfully
    GLint isCompiled = 0;
    glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &isCompiled);

    // if it didnt we print a error to the console
    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        compiled = GL_FALSE;
        glGetShaderiv(fragmentID, GL_INFO_LOG_LENGTH, &maxLength);

        GLchar * errorLog = new char[maxLength];
        glGetShaderInfoLog(fragmentID, maxLength, &maxLength, &errorLog[0]);
        printf(errorLog);
    } 
}

GLvoid GPU_Program::setFloat(GLchar * nm, float v)
{
    // Get its ID
    GLuint ID = glGetUniformLocation(programID, "alpha");
    // and set the value
    glUniform1f(ID, v);
}

void GPU_Program::setVertexSource(const char * source)
{
    //  if the source is not a filename
    if (std::string(source).find(".glsl") == std::string::npos)
    {
        // we just load it directly into the shader
        glShaderSourceARB(vertexID, 1, &source, NULL);
        glCompileShader(vertexID);
    }
    else
    {
        // set first line
        std::ifstream file(source);
        if (file.is_open())
        {
            // set first line
            std::string src = SHADER_VERSION;
            // then loop through each line adding it to the source
            while (!file.eof())
            {
                std::string line = "";
                std::getline(file, line);
                src += line;
            }

            // then grab the full source from the string
            const GLchar * c_source = src.c_str();

            // and send it to the shader
            glShaderSourceARB(vertexID, 1, &c_source, NULL);
            glCompileShader(vertexID);
        }
        else
        {
            // print error when we cant open the file
            std::cout << "Error opening file " << source << std::endl;
        }
    }

    // then here we always check to make sure it compiled successfully
    GLint isCompiled = 0;
    glGetShaderiv(vertexID, GL_COMPILE_STATUS, &isCompiled);
    
    // if it didnt we print a error to the console
    if(isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        compiled = GL_FALSE;
        glGetShaderiv(vertexID, GL_INFO_LOG_LENGTH, &maxLength);  

        GLchar * errorLog = new char[maxLength];
        glGetShaderInfoLog(vertexID, maxLength, &maxLength, &errorLog[0]);
        printf(errorLog);
    }
}

// this function just attaches the programs
GLboolean GPU_Program::compile()
{
    // providing both shaders have compiled
    if (compiled == GL_TRUE) {
        // attach them to our opengl progarm object
        glAttachShader(programID, vertexID);
        glAttachShader(programID, fragmentID);
        glLinkProgram(programID);
    }

    // then return the compiled status to the user
    return compiled;
}

// here we just set a vectors value in the shader
GLvoid GPU_Program::setVector(GLchar * nm, glm::vec2 v)
{
    // get the ID for the variable
    GLuint ID = glGetUniformLocation(programID, nm);
    // and set its value
    glUniform2fv(ID, 1, glm::value_ptr(v));
}

// here we just set a matrices value in the shader
void GPU_Program::setMatrix(GLchar * nm, float * m)
{
    // get the ID for the variable
    GLuint ID = glGetUniformLocation(programID, nm);
    // and set its value
    glUniformMatrix4fv(ID, 1, GL_FALSE, m);
}

// This function just binds a texture for the user
void GPU_Program::bind(GLuint textureID)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

// here we just set a integers value in the shader
GLvoid GPU_Program::setInteger(GLchar * nm, int v)
{
    // Get its ID
    GLuint ID = glGetUniformLocation(programID, nm);
    // and set the value
    glUniform1i(ID, v);
}

// here we just prepare the program by binding the objects VAO
void GPU_Program::prepare(GLuint objectID)
{
    glBindVertexArray(objectID);
}

// This function simply binds the program ready for use
void GPU_Program::bindShader() 
{
    glUseProgram(programID);
}

// This function simply calls a draw call and renders the object onto the screen
void GPU_Program::run(GLuint type, GLuint start, GLuint count)
{
    glDrawArrays(type, start, count);
}

// two simpler functions of the one above
void GPU_Program::run(GLuint start, GLuint count)
{
    run(GL_TRIANGLES, start, count);
}

void GPU_Program::run(GLuint count)
{
    run(GL_TRIANGLES, 0, count);
}

// returns the program ID
GLuint GPU_Program::getID()
{
    return programID;
}

// sets the program to NULL
void GPU_Program::unbindShader() 
{
    glUseProgram(NULL);
}
