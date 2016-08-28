
#include "GPU_Program.h"

GPU_Program::GPU_Program()
    : compiled(GL_TRUE)
{
    programID = glCreateProgram();
    fragmentID = glCreateShader(GL_FRAGMENT_SHADER);
    vertexID = glCreateShader(GL_VERTEX_SHADER);
}

GPU_Program::~GPU_Program()
{
    glDeleteProgram(programID);
    glDeleteShader(fragmentID);
    glDeleteShader(vertexID);
}

void GPU_Program::setFragmentSource(const char * source)
{
    if (std::string(source).find(".glsl") == std::string::npos)
    {
        glShaderSource(fragmentID, 1, &source, nullptr);
        glCompileShader(fragmentID);
    }
    else
    {
        std::ifstream file(source);

        if (file.is_open())
        {
            std::string src = SHADER_VERSION;

            while (!file.eof())
            {
                std::string line = "";
                getline(file, line);
                src += line;
            }
    
            const GLchar * c_source = src.c_str();
            glShaderSourceARB(fragmentID, 1, &c_source, nullptr);
            glCompileShader(fragmentID);
        }
        else
        {
            std::cout << "Error opening file " << source << std::endl;
        }
    }

    auto isCompiled = 0;
    glGetShaderiv(fragmentID, GL_COMPILE_STATUS, &isCompiled);

    if (isCompiled == GL_FALSE) 
    {
        auto maxLength = 0;
        compiled = GL_FALSE;
        glGetShaderiv(fragmentID, GL_INFO_LOG_LENGTH, &maxLength);

        auto errorLog = new char[maxLength];
        glGetShaderInfoLog(fragmentID, maxLength, &maxLength, &errorLog[0]);
        std::cout << errorLog << std::endl;
        delete[] errorLog;    
    } 
}

GLvoid GPU_Program::setFloat(GLchar * nm, float v)
{
    GLuint ID = glGetUniformLocation(programID, "alpha");
    glUniform1f(ID, v);
}

void GPU_Program::setVertexSource(const char * source)
{
    if (std::string(source).find(".glsl") == std::string::npos)
    {
        // we just load it directly into the shader
        glShaderSourceARB(vertexID, 1, &source, nullptr);
        glCompileShader(vertexID);
    }
    else
    {
        std::ifstream file(source);

        if (file.is_open())
        {
            std::string src = SHADER_VERSION;

            while (!file.eof())
            {
                std::string line = "";
                std::getline(file, line);
                src += line;
            }

            const GLchar * c_source = src.c_str();
            glShaderSourceARB(vertexID, 1, &c_source, nullptr);
            glCompileShader(vertexID);
        }
        else
        {
            std::cout << "Error opening file " << source << std::endl;
        }
    }

    auto isCompiled = 0;
    glGetShaderiv(vertexID, GL_COMPILE_STATUS, &isCompiled);
    
    if(isCompiled == GL_FALSE) 
    {
        auto maxLength = 0;
        compiled = GL_FALSE;
        glGetShaderiv(vertexID, GL_INFO_LOG_LENGTH, &maxLength);  

        auto errorLog = new char[maxLength];
        glGetShaderInfoLog(vertexID, maxLength, &maxLength, &errorLog[0]);
        std::cout << errorLog << std::endl;
        delete[] errorLog;
    }
}

GLboolean GPU_Program::compile()
{
    if (compiled == GL_TRUE) 
    {
        glAttachShader(programID, vertexID);
        glAttachShader(programID, fragmentID);
        glLinkProgram(programID);
    }

    return compiled;
}

GLvoid GPU_Program::setVector(GLchar * nm, glm::vec2 v)
{
    GLuint ID = glGetUniformLocation(programID, nm);
    glUniform2fv(ID, 1, glm::value_ptr(v));
}

void GPU_Program::setMatrix(GLchar * nm, float * m)
{
    GLuint ID = glGetUniformLocation(programID, nm);
    glUniformMatrix4fv(ID, 1, GL_FALSE, m);
}

void GPU_Program::bind(GLuint textureID)
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

GLvoid GPU_Program::setInteger(GLchar * nm, int v)
{
    GLuint ID = glGetUniformLocation(programID, nm);
    glUniform1i(ID, v);
}

void GPU_Program::prepare(GLuint objectID)
{
    glBindVertexArray(objectID);
}

void GPU_Program::bindShader() 
{
    glUseProgram(programID);
}

void GPU_Program::run(GLuint type, GLuint start, GLuint count)
{
    glDrawArrays(type, start, count);
}

void GPU_Program::run(GLuint start, GLuint count)
{
    run(GL_TRIANGLES, start, count);
}

void GPU_Program::run(GLuint count)
{
    run(GL_TRIANGLES, 0, count);
}

GLuint GPU_Program::getID()
{
    return programID;
}

void GPU_Program::unbindShader() 
{
    glUseProgram(NULL);
}
