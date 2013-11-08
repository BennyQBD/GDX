#include "shader.h"
#include <sstream>
#include <cassert>

Shader::Shader()
{
	m_Program = 0;
	m_Shaders = std::vector<GLuint>();
	m_Uniforms = std::map<std::string, GLuint>();
	m_Cleanup = false;
}

Shader::Shader(const std::string& fileName)
{
    m_Program = glCreateProgram();
    std::string vertexShaderFile = fileName + ".vs";
    std::string fragmentShaderFile = fileName + ".fs";
    
    AddShader(LoadShader(vertexShaderFile), GL_VERTEX_SHADER);
    AddShader(LoadShader(fragmentShaderFile), GL_FRAGMENT_SHADER);
    
    CompileShader();
}

Shader::~Shader()
{
	if(m_Cleanup)
	{
		for(std::vector<GLuint>::iterator it = m_Shaders.begin(); it != m_Shaders.end(); ++it) 
        {
            glDetachShader(m_Program,*it);
            glDeleteShader(*it);
        }
        glDeleteProgram(m_Program);
	}
}

//TODO: This needs to be a deep copy!
Shader::Shader(const Shader& shader)
{
	m_Program = shader.m_Program;
	m_Shaders = shader.m_Shaders;
	m_Uniforms = shader.m_Uniforms;
	m_Cleanup = shader.m_Cleanup;
	shader.m_Cleanup = false;
}

void Shader::operator=(const Shader& shader)
{
	m_Program = shader.m_Program;
	m_Shaders = shader.m_Shaders;
	m_Uniforms = shader.m_Uniforms;
	m_Cleanup = shader.m_Cleanup;
	shader.m_Cleanup = false;
}

void Shader::Bind()
{
    glUseProgram(m_Program);
}

void Shader::AddUniform(const std::string& uniform)
{
    GLuint location = glGetUniformLocation(m_Program, uniform.c_str());

    assert(location != 0xFFFFFFFF);

    m_Uniforms.insert(std::pair<std::string,GLuint>(uniform, location));
}

void Shader::CompileShader()
{
    glLinkProgram(m_Program);
    CheckShaderError(m_Program, GL_LINK_STATUS, true, "Error linking shader program");
}

void Shader::ValidateShader()
{
    glValidateProgram(m_Program);
    CheckShaderError(m_Program, GL_VALIDATE_STATUS, true, "Invalid shader program");
}

void Shader::CompileAndValidateShader()
{
    CompileShader();
    ValidateShader();
}

void Shader::AddShader(const std::string& text, GLuint type)
{
    GLuint shader = glCreateShader(type);
    
    std::stringstream errorMessage;
    errorMessage << "Error compiling shader type " << type;

    if(shader == 0)
        Display::Error(errorMessage.str());

    const GLchar* p[1];
    p[0] = text.c_str();
    GLint lengths[1];
    lengths[0] = text.length();

    glShaderSource(shader, 1, p, lengths);
    glCompileShader(shader);

    CheckShaderError(shader, GL_COMPILE_STATUS, false, errorMessage.str());
    
    glAttachShader(m_Program, shader);
    m_Shaders.push_back(shader);
}

void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
    GLint success = 0;
    GLchar error[1024] = { 0 };

    if(isProgram)
        glGetProgramiv(shader, flag, &success);
    else
        glGetShaderiv(shader, flag, &success);

    if(success == GL_FALSE)
    {
        if(isProgram)
            glGetProgramInfoLog(shader, sizeof(error), NULL, error);
        else
            glGetShaderInfoLog(shader, sizeof(error), NULL, error);

        std::stringstream message;
        message << errorMessage << ": '" << error << "'";
        Display::Error(message.str());
    }
}
