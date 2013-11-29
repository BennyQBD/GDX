#include "shader.h"
#include "util.h"

#include <sstream>
#include <cassert>
#include <iostream>
#include <stdexcept>

std::map<std::string, Shader*> Shader::ShaderPrograms = std::map<std::string, Shader*>();

Shader* Shader::Get(const std::string& name)
{
	std::map<std::string, Shader*>::iterator it = ShaderPrograms.find(name);
	if(it != ShaderPrograms.end())
		return it->second;

	ShaderPrograms.insert(std::pair<std::string, Shader*>(name, new Shader(name)));
    return ShaderPrograms.at(name);
    //try 
    //{
    //    return ShaderPrograms.at(name);
    //}
    //catch (const std::out_of_range& oor) 
    //{
    //    ShaderPrograms.insert(std::pair<std::string, Shader*>(name, new Shader(name)));
    //    return ShaderPrograms.at(name);
    //}
}

void Shader::Delete(const std::string& name)
{
	std::map<std::string, Shader*>::iterator it = ShaderPrograms.find(name);
	if(it != ShaderPrograms.end()) 
	{
		delete it->second;
		ShaderPrograms.erase(it);
	} 
}

void Shader::DeleteAll()
{
    for (std::map<std::string, Shader*>::iterator it=ShaderPrograms.begin(); it!=ShaderPrograms.end(); ++it)
        delete it->second;
}

std::string Shader::LoadShader(const std::string& fileName)
{
    std::ifstream file;
    file.open(("./res/shaders/" + fileName).c_str());

    std::string output;
    std::string line;

    if(file.is_open())
    {
        while(file.good())
        {
            getline(file, line);

			if(line.find("#include") == std::string::npos)
				output.append(line + "\n");
			else
			{
				std::string includeFileName = Util::Split(line, ' ')[1];
				includeFileName = includeFileName.substr(1,includeFileName.length() - 2);

				std::string toAppend = LoadShader(includeFileName);
				output.append(toAppend + "\n");
			}
        }
    }
    else
    {
        Engine::GetDisplay()->Error("Unable to load shader: " + fileName);
    }

    return output;
}

Shader::Shader(const std::string& fileName)
{
    m_Program = 0;
	m_Shaders = std::vector<unsigned int>();
	m_Uniforms = std::vector<UniformData>();
	m_IsValidated = false;
    
    std::string shaderText = LoadShader(fileName + ".glsl");

    m_Shaders.push_back(Engine::GetRenderer()->CreateVertexShader(shaderText));
	m_Shaders.push_back(Engine::GetRenderer()->CreateFragmentShader(shaderText));
    m_Program = Engine::GetRenderer()->CreateShaderProgram(&m_Shaders[0], m_Shaders.size());
    
    //Find and add uniforms
    AddUniforms(shaderText);
}

Shader::~Shader()
{
	Engine::GetRenderer()->DeleteShaderProgram(m_Program, &m_Shaders[0], m_Shaders.size());
}

void Shader::Bind()
{
    Engine::GetRenderer()->BindShaderProgram(m_Program);
}

void Shader::AddUniforms(const std::string& shaderText)
{
    static const std::string UNIFORM_KEY = "uniform";
    
    size_t uniformLocation = shaderText.find(UNIFORM_KEY);
    while(uniformLocation != std::string::npos)
    {
        bool isCommented = false;
        size_t lastLineEnd = shaderText.rfind(";", uniformLocation);
        
        if(lastLineEnd != std::string::npos)
        {
            std::string potentialCommentSection = shaderText.substr(lastLineEnd,uniformLocation - lastLineEnd);
            isCommented = potentialCommentSection.find("//") != std::string::npos;
        }
        
        if(!isCommented)
        {
            size_t begin = uniformLocation + UNIFORM_KEY.length();
            size_t end = shaderText.find(";", begin);
            
            std::string uniformLine = shaderText.substr(begin + 1, end-begin - 1);
            
            begin = uniformLine.find(" ");
            std::string uniformName = uniformLine.substr(begin + 1);
            std::string uniformType = uniformLine.substr(0, begin);
            
            AddUniform(uniformName, uniformType);
        }
        uniformLocation = shaderText.find(UNIFORM_KEY, uniformLocation + UNIFORM_KEY.length());
    }
}

void Shader::AddUniform(const std::string& uniform, const std::string& uniformType)
{
    unsigned int location = Engine::GetRenderer()->GetShaderProgramUniformLocation(m_Program, uniform);

    if(location == 0xFFFFFFFF)
    {
        Engine::GetDisplay()->Error("Could not find uniform: " + uniform + " " + uniformType);
    }
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
		Engine::GetRenderingEngine()->UpdateUniform(&(*it), transform, material);
    
    if(!m_IsValidated)
        ValidateShader();
}
