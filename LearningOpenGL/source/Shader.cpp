#include "Shader.h"
#include "Renderer.h"
#include <iostream>
#include <fstream>
#include <string> 
#include <sstream>




unsigned int Shader::GetUniformLocation(const std::string& name)
{
	return 0;
}

Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath), m_RendererID(0)
{
	CompileShader();
}

Shader::~Shader()
{
}

ShaderCodes Shader::ParseShaderSource(const std::string& filepath)
{
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream shaderBuffer[2]; // 0-> vertex, 1-> fragment

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    ShaderType curType = ShaderType::NONE;


    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                curType = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                curType = ShaderType::FRAGMENT;
            }
        }
        else
        {
            shaderBuffer[(int)curType] << line << '\n';
        }
    }
    return { shaderBuffer[0].str(),shaderBuffer[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{

    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Error while compiling the " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader: " << std::endl << message << std::endl;
        glDeleteShader(id);
        return 0;


    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vertexShaderID = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fragmentShaderID = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vertexShaderID);
    glAttachShader(program, fragmentShaderID);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return program;
}

void Shader::Bind() const
{
}

void Shader::Unbind() const
{
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
}
