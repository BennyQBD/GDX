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

static void String_ReplaceKey(std::string& replaceIn, size_t replacementStart, const std::string& replacementValue, const std::string& endKey, int startLocation)
{
    size_t replacementEnd = replaceIn.find(endKey, startLocation) - replacementStart;

    replaceIn.replace(replacementStart,replacementEnd,replacementValue);
}

static void String_FindAndReplace(std::string& replaceIn, const std::string& replacementKey, const std::string& replacementValue, const std::string& endKey = "", int startLocation = 0)
{
    size_t replacementStart = replaceIn.find(replacementKey, startLocation);
    String_ReplaceKey(replaceIn,replaceIn.find(replacementKey, startLocation),replacementValue,endKey,replacementStart + replacementKey.length());
}

static void String_ReplaceAll(std::string& replaceIn, const std::string& replacementKey, const std::string& replacementValue, const std::string& endKey = "", int startLocation = 0, bool insertCounter = false)
{
    static std::string COUNTER_KEY = "%d";
    
    int numReplaced = 0;
    size_t replacementLocation = replaceIn.find(replacementKey, startLocation);
    
    size_t counterLocation = 0;
    std::string newReplacementStart = "";
    std::string newReplacementEnd = "";
    
    if(insertCounter)
    {
        counterLocation = replacementValue.find(COUNTER_KEY);
        newReplacementStart = replacementValue.substr(0, counterLocation);
        newReplacementEnd = replacementValue.substr(counterLocation + COUNTER_KEY.length());
    }
    
    while(replacementLocation != std::string::npos)
    {
        if(insertCounter)
        {
            std::stringstream newReplacement;
            
            newReplacement << newReplacementStart << numReplaced << newReplacementEnd;
                
            replaceIn.replace(replacementLocation, replacementKey.length(), newReplacement.str());
        }
        else
            String_ReplaceKey(replaceIn, replacementLocation, replacementValue, endKey, replacementLocation + replacementKey.length());
                
        replacementLocation = replaceIn.find(replacementKey, replacementLocation + replacementValue.length());
        numReplaced++;
    }
}

static size_t String_FindClosingBrace(const std::string& text, size_t initialBraceLocation)
{
    size_t currentLocation = initialBraceLocation + 1;
    
    while(currentLocation != std::string::npos)
    {
        size_t nextOpening = text.find("{", currentLocation);
        size_t nextClosing = text.find("}", currentLocation);
        
        if(nextClosing < nextOpening || nextOpening == std::string::npos)
            return nextClosing;

        currentLocation = nextClosing;
    }
    
    Display::Error("Error: Shader is missing a closing brace!");
    assert(0 == 0);
    
    return -1;
}

Shader::Shader(const std::string& fileName)
{
    m_Program = 0;
	m_Shaders = std::vector<unsigned int>();
	m_Uniforms = std::vector<UniformData>();
	m_IsValidated = false;
	m_Cleanup = true;
    
//        std::string vertexShaderFile = fileName + ".vs";
//        std::string fragmentShaderFile = fileName + ".fs";
//        
//        m_Shaders.push_back(Engine::GetRenderer()->CreateVertexShader(LoadShader(vertexShaderFile)));
//        m_Shaders.push_back(Engine::GetRenderer()->CreateFragmentShader(LoadShader(fragmentShaderFile)));
//        
//        m_Program = Engine::GetRenderer()->CreateShaderProgram(&m_Shaders[0], m_Shaders.size());
//        
////        AddUniform("MVP", "mat4");
////        AddUniform("transform", "mat4");
////        AddUniform("diffuse", "sampler2D");
//        
//        return;
    
    std::string shaderText = LoadShader(fileName + ".glsl");
    std::string version = GetSpecifiedVersion(shaderText);
    
    size_t vertexShaderKeyLocation = shaderText.find("void VSmain()");
    size_t fragmentShaderKeyLocation = shaderText.find("void FSmain()");
    
    if(vertexShaderKeyLocation != std::string::npos)
    {
        std::string vertexShaderText = std::string(shaderText);
        String_FindAndReplace(vertexShaderText, "void VSmain()", "void main()");
        
        if(fragmentShaderKeyLocation != std::string::npos)
            EraseShaderFunction(vertexShaderText, "void FSmain()");
        
        if(version.compare("330") == 0)
        {
            String_ReplaceAll(vertexShaderText, "attribute", "layout(location = %d) in", "", 0, true);
            String_ReplaceAll(vertexShaderText, "varying", "out");
        }
        
        std::cout << vertexShaderText << std::endl;
        m_Shaders.push_back(Engine::GetRenderer()->CreateVertexShader(vertexShaderText));
    }
    
    if(fragmentShaderKeyLocation != std::string::npos)
    {
        std::string fragmentShaderText = std::string(shaderText);
        String_FindAndReplace(fragmentShaderText, "void FSmain()", "void main()");
        
        if(vertexShaderKeyLocation != std::string::npos)
            EraseShaderFunction(fragmentShaderText, "void VSmain()");
        
        String_ReplaceAll(fragmentShaderText, "attribute", "", "\n");
        
        if(version.compare("330") == 0)
            String_ReplaceAll(fragmentShaderText, "varying", "in");
        
        std::cout << fragmentShaderText << std::endl;
        m_Shaders.push_back(Engine::GetRenderer()->CreateFragmentShader(fragmentShaderText));
    }
    
    m_Program = Engine::GetRenderer()->CreateShaderProgram(&m_Shaders[0], m_Shaders.size());
    
    //Find and add uniforms
    AddUniforms(shaderText);
}

Shader::~Shader()
{
	if(m_Cleanup)
	    Engine::GetRenderer()->DeleteShaderProgram(m_Program, &m_Shaders[0], m_Shaders.size());
}

std::string Shader::GetSpecifiedVersion(const std::string& shaderText)
{
    static const std::string VERSION_KEY = "#version ";
    
    size_t versionLocation = shaderText.find(VERSION_KEY);
    size_t versionNumberStart = versionLocation + VERSION_KEY.length();
    size_t versionNumberEnd = shaderText.find("\n",versionNumberStart) - versionNumberStart;
    return shaderText.substr(versionNumberStart,versionNumberEnd);     
}

void Shader::EraseShaderFunction(std::string& text, const std::string& functionHeader)
{
    size_t begin = text.find(functionHeader);
    
    if(begin == std::string::npos)
    {
        Display::Error("Error: Shader function does not exist!");
        assert(0 == 0);
    }
    
    size_t end = String_FindClosingBrace(text,text.find("{", begin));
    text.erase(begin, end-begin + 1);
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
            //std::cout << uniformLine << std::endl;
        }
        uniformLocation = shaderText.find(UNIFORM_KEY, uniformLocation + UNIFORM_KEY.length());
    }
}

void Shader::AddUniform(const std::string& uniform, const std::string& uniformType)
{
    unsigned int location = Engine::GetRenderer()->GetShaderProgramUniformLocation(m_Program, uniform);

    if(location == 0xFFFFFFFF)
    {
        Display::Error("Could not find uniform: " + uniform + " " + uniformType);
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
    {
        if(it->Name.compare("MVP") == 0)
            Engine::GetRenderer()->SetUniformMatrix4f(it->Location, transform.GetMVP());
		
		if(it->Name.compare("transform") == 0)
            Engine::GetRenderer()->SetUniformMatrix4f(it->Location, transform.GetModel());
		
		if(it->Type.compare("sampler2D") == 0)
		{
		    int unit = Material::GetTextureUnit(it->Name);
            material.GetTexture(it->Name)->Bind(unit);
            Engine::GetRenderer()->SetUniformInt(it->Location, unit);
		}
    }
    
    if(!m_IsValidated)
        ValidateShader();
}
