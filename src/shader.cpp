#include "shader.h"

#include <sstream>
#include <cassert>

Shader::Shader()
{
	m_Program = 0;
	m_Shaders = std::vector<unsigned int>();
	m_Uniforms = std::map<std::string, unsigned int>();
	m_Cleanup = false;
}

Shader::Shader(const std::string& fileName)
{
    std::string vertexShaderFile = fileName + ".vs";
    std::string fragmentShaderFile = fileName + ".fs";
    
    m_Shaders.push_back(Engine::GetRenderer()->CreateVertexShader(LoadShader(vertexShaderFile)));
    m_Shaders.push_back(Engine::GetRenderer()->CreateFragmentShader(LoadShader(fragmentShaderFile)));
    
    m_Program = Engine::GetRenderer()->CreateShaderProgram(&m_Shaders[0], m_Shaders.size());;
}

Shader::~Shader()
{
	if(m_Cleanup)
	    Engine::GetRenderer()->DeleteShaderProgram(m_Program, &m_Shaders[0], m_Shaders.size());
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
    Engine::GetRenderer()->BindShaderProgram(m_Program);
}

void Shader::AddUniform(const std::string& uniform)
{
    unsigned int location = Engine::GetRenderer()->GetShaderProgramUniformLocation(m_Program, uniform);

    assert(location != 0xFFFFFFFF);

    m_Uniforms.insert(std::pair<std::string,unsigned int>(uniform, location));
}

void Shader::ValidateShader()
{
    Engine::GetRenderer()->ValidateShaderProgram(m_Program);
}
