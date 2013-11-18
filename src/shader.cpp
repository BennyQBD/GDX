#include "shader.h"

#include <sstream>
#include <cassert>
#include <iostream>
#include <stdexcept>

std::map<std::string, Shader*> Shader::ShaderPrograms = std::map<std::string, Shader*>();

Shader* Shader::Get(const std::string& name)
{
    try 
    {
        return ShaderPrograms.at(name);
    }
    catch (const std::out_of_range& oor) 
    {
        ShaderPrograms.insert(std::pair<std::string, Shader*>(name, new Shader(name)));
        return ShaderPrograms.at(name);
    }
}

void Shader::RemoveShaders()
{
    for (std::map<std::string, Shader*>::iterator it=ShaderPrograms.begin(); it!=ShaderPrograms.end(); ++it)
        delete it->second;
}

Shader::Shader()
{
	m_Program = 0;
	m_Shaders = std::vector<unsigned int>();
	m_Uniforms = std::vector<UniformData>();
	m_IsValidated = false;
	m_Cleanup = false;
}

Shader::Shader(const std::string& fileName)
{
    m_Program = 0;
	m_Shaders = std::vector<unsigned int>();
	m_Uniforms = std::vector<UniformData>();
	m_IsValidated = false;
	m_Cleanup = true;
    
    static const std::string MAIN_METHOD_KEY = "void main()";
    static const std::string VERTEX_MAIN_KEY = "void VSmain()";
    static const std::string FRAGMENT_MAIN_KEY = "void FSmain()";
    
    std::string shaderText = LoadShader(fileName + ".glsl");
    
    size_t vertexShaderKeyLocation = shaderText.find(VERTEX_MAIN_KEY);
    size_t fragmentShaderKeyLocation = shaderText.find(FRAGMENT_MAIN_KEY);
    
    if(vertexShaderKeyLocation != std::string::npos)
    {
        std::string vertexShaderText = std::string(shaderText);
        vertexShaderText.replace(vertexShaderKeyLocation,VERTEX_MAIN_KEY.length(),MAIN_METHOD_KEY);
        
        //Removes Fragment Shader Main Function
        if(fragmentShaderKeyLocation != std::string::npos)
        {
            //TODO: Find a more general way to find the end of the method
            size_t begin = vertexShaderText.find(FRAGMENT_MAIN_KEY);
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
        fragmentShaderText.replace(fragmentShaderKeyLocation,FRAGMENT_MAIN_KEY.length(),MAIN_METHOD_KEY);
        
        //Removes Vertex Shader Main Function
        if(vertexShaderKeyLocation != std::string::npos)
        {
            //TODO: Find a more general way to find the end of the method
            size_t begin = fragmentShaderText.find(VERTEX_MAIN_KEY);
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
    
    //Find and add uniforms
    static const std::string UNIFORM_KEY = "uniform";
    
    size_t uniformLocation = shaderText.find(UNIFORM_KEY);
    while(uniformLocation != std::string::npos)
    {
        size_t begin = uniformLocation + UNIFORM_KEY.length();
        size_t end = shaderText.find(";", begin);
        
        std::string uniformLine = shaderText.substr(begin + 1, end-begin - 1);
        
        begin = uniformLine.find(" ");
        std::string uniformName = uniformLine.substr(begin + 1);
        std::string uniformType = uniformLine.substr(0, begin);
        
        AddUniform(uniformName, uniformType);
        //std::cout << uniformLine << std::endl;
        
        uniformLocation = shaderText.find(UNIFORM_KEY, uniformLocation + 1);
    }
}

Shader::~Shader()
{
	if(m_Cleanup)
	    Engine::GetRenderer()->DeleteShaderProgram(m_Program, &m_Shaders[0], m_Shaders.size());
}

void Shader::Bind()
{
    Engine::GetRenderer()->BindShaderProgram(m_Program);
}

void Shader::AddUniform(const std::string& uniform, const std::string& uniformType)
{
    unsigned int location = Engine::GetRenderer()->GetShaderProgramUniformLocation(m_Program, uniform);

    assert(location != 0xFFFFFFFF);

    m_Uniforms.push_back(UniformData(location, uniformType, uniform));
}

void Shader::ValidateShader()
{
    Engine::GetRenderer()->ValidateShaderProgram(m_Program);
    m_IsValidated = true;
}

void Shader::Update(Transform& transform, Material& material)
{
    for(std::vector<UniformData>::iterator it = m_Uniforms.begin(); it != m_Uniforms.end(); ++it)
    {
        if(it->Name.compare("MVP") == 0)
            Engine::GetRenderer()->SetUniformMatrix4f(it->Location, transform.GetMVP());
		
		if(it->Name.compare("transform") == 0)
            Engine::GetRenderer()->SetUniformMatrix4f(it->Location, transform.GetModel());
		
		if(it->Type.compare("sampler2D") == 0)
            material.GetTexture(it->Name)->Bind(Material::GetTextureUnit(it->Name));
    }
    
    if(!m_IsValidated)
        ValidateShader();
}
