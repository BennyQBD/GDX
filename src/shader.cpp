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

#include <iostream>

Shader::Shader(const std::string& fileName)
{
    static const std::string MainMethodKey = "void main()";
    static const std::string VertexShaderMainMethodKey = "void VSmain()";
    static const std::string FragmentShaderMainMethodKey = "void FSmain()";
    
    std::string shaderText = LoadShader(fileName + ".glsl");
    
    size_t vertexShaderKeyLocation = shaderText.find(VertexShaderMainMethodKey);
    size_t fragmentShaderKeyLocation = shaderText.find(FragmentShaderMainMethodKey);
    
    if(vertexShaderKeyLocation != std::string::npos)
    {
        std::string vertexShaderText = std::string(shaderText);
        vertexShaderText.replace(vertexShaderKeyLocation,VertexShaderMainMethodKey.length(),MainMethodKey);
        
        //Removes Fragment Shader Main Function
        if(fragmentShaderKeyLocation != std::string::npos)
        {
            //TODO: Find a more general way to find the end of the method
            size_t begin = vertexShaderText.find(FragmentShaderMainMethodKey);
            size_t end = vertexShaderText.find_last_of("}");
            if (end != std::string::npos && begin <= end)
                vertexShaderText.erase(begin, end-begin + 1);
        }
        
        //std::cout << vertexShaderText << std::endl;
        m_Shaders.push_back(Engine::GetRenderer()->CreateVertexShader(vertexShaderText));
    }
    
    if(fragmentShaderKeyLocation != std::string::npos)
    {
        std::string fragmentShaderText = std::string(shaderText);
        fragmentShaderText.replace(fragmentShaderKeyLocation,FragmentShaderMainMethodKey.length(),MainMethodKey);
        
        //Removes Vertex Shader Main Function
        if(vertexShaderKeyLocation != std::string::npos)
        {
            //TODO: Find a more general way to find the end of the method
            size_t begin = fragmentShaderText.find(VertexShaderMainMethodKey);
            size_t end = fragmentShaderText.find_last_of("}", fragmentShaderKeyLocation);
            if (end != std::string::npos && begin <= end)
                fragmentShaderText.erase(begin, end-begin + 1);
        }
        
        //Removes Attributes (invalid in fragment shaders)
        size_t attributeLocation = fragmentShaderText.find("attribute");
        while(attributeLocation != std::string::npos)
        {
            size_t begin = attributeLocation;
            size_t end = fragmentShaderText.find(";", begin);
            if (end != std::string::npos)
                fragmentShaderText.erase(begin, end-begin + 1);
            
            attributeLocation = fragmentShaderText.find("attribute");
        }
        
        //std::cout << fragmentShaderText << std::endl;
        m_Shaders.push_back(Engine::GetRenderer()->CreateFragmentShader(fragmentShaderText));
    }
    
    m_Program = Engine::GetRenderer()->CreateShaderProgram(&m_Shaders[0], m_Shaders.size());
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
