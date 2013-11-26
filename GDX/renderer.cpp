#include "renderer.h"
#include "mesh.h"
#include "math3d.h"
#include "display.h"

#include <GL/glew.h>
#include <stdio.h>
#include <sstream>
#include <cassert>

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

std::string GetSpecifiedVersion(const std::string& shaderText)
{
    static const std::string VERSION_KEY = "#version ";
    
    size_t versionLocation = shaderText.find(VERSION_KEY);
    size_t versionNumberStart = versionLocation + VERSION_KEY.length();
    size_t versionNumberEnd = shaderText.find("\n",versionNumberStart) - versionNumberStart;
    return shaderText.substr(versionNumberStart,versionNumberEnd);     
}

void EraseShaderFunction(std::string& text, const std::string& functionHeader)
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

void Renderer::Init()
{
    GLenum glewError = glewInit(); 
    if(glewError != GLEW_OK) 
    { 
        printf("Error initializing GLEW: %s\n", glewGetErrorString(glewError)); 
        return; 
    }
    
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_DEPTH_CLAMP);
}

void Renderer::SetClearColor(float r, float g, float b, float a)
{
    glClearColor(r,g,b,a);
}

void Renderer::ClearScreenAndDepth()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::ClearScreen()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::ClearDepth()
{
    glClear(GL_DEPTH_BUFFER_BIT);
}

static inline unsigned int CreateDataBuffer(void* data, int dataSize, bool isStatic, int bindHint)
{
    unsigned int buffer;
    int hint = GL_STATIC_DRAW;
    glGenBuffers(1, &buffer);
    
    if(!isStatic)
        hint = GL_DYNAMIC_DRAW;//TODO: Make sure this hint is appropriate!
    
    glBindBuffer(bindHint, buffer);
    glBufferData(bindHint, dataSize, data, hint);
    
    return buffer;
}

unsigned int Renderer::CreateVertexBuffer(void* data, int dataSize, bool isStatic)
{
    return CreateDataBuffer(data, dataSize, isStatic, GL_ARRAY_BUFFER);
}

unsigned int Renderer::CreateIndexBuffer(void* data, int dataSize, bool isStatic)
{
    return CreateDataBuffer(data, dataSize, isStatic, GL_ELEMENT_ARRAY_BUFFER);
}

void Renderer::DeleteBuffer(unsigned int buffer)
{
    glDeleteBuffers(1, &buffer);
}

void Renderer::DrawTriangles(unsigned int vertexBuffer, unsigned int indexBuffer, int nIndices, const VertexFormat& format)
{
    for(int i = 0; i < format.nElements; i++)
        glEnableVertexAttribArray(i);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    
    int memOffset = 0;
    for(int i = 0; i < format.nElements; i++)
    {
        int n = format.ElementSizes[i] / sizeof(float);
        glVertexAttribPointer(i, n, GL_FLOAT, GL_FALSE, format.VertexSize, (GLvoid*)memOffset);
        memOffset += format.ElementSizes[i];
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glDrawElements(GL_TRIANGLES, nIndices, GL_UNSIGNED_INT, 0);

    for(int i = 0; i < format.nElements; i++)
        glDisableVertexAttribArray(i);
}

static inline void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
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

static inline unsigned int CreateShader(const std::string& text, unsigned int type)
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
    
    return shader;
}

size_t String_NextToken(const std::string& text, size_t startLocation, std::string* token_output)
{
	std::string lastCharacter = text.substr(startLocation, 1);
	if(lastCharacter.compare(";") == 0
	   || lastCharacter.compare("(") == 0)
	{
		(*token_output) = lastCharacter;
		return startLocation + 1;
	}

	size_t nextWhitespace = text.find(" ", startLocation);
	size_t nextNewLine = text.find("\n", startLocation);
	size_t tokenEnd = nextWhitespace;

	if(nextNewLine < tokenEnd)
		tokenEnd = nextNewLine;

	(*token_output) = text.substr(startLocation, tokenEnd - startLocation);
	startLocation = tokenEnd + 1;

	if((*token_output).length() > 0)
	{
		std::string outputWithoutTheEnd = (*token_output).substr((*token_output).length() - 1, 1);
		if(outputWithoutTheEnd.compare(";") == 0
			|| outputWithoutTheEnd.compare("(") == 0)
		{
			(*token_output) = (*token_output).substr(0, (*token_output).length() - 1);
			startLocation -= 2;
		}
	}

	if((*token_output).compare("\n") == 0
	   || (*token_output).compare(" ") == 0 
	   || (*token_output).compare("") == 0 
	   && startLocation != 0)
	{
		return String_NextToken(text, startLocation, token_output);
	}

	return startLocation;
}

unsigned int Renderer::CreateVertexShader(const std::string& shaderText)
{
    std::string version = GetSpecifiedVersion(shaderText);
    
    size_t vertexShaderKeyLocation = shaderText.find("void VSmain()");
    size_t fragmentShaderKeyLocation = shaderText.find("void FSmain()");
    
    std::string vertexShaderText = std::string(shaderText);
    String_FindAndReplace(vertexShaderText, "void VSmain()", "void main()");
        
    if(fragmentShaderKeyLocation != std::string::npos)
        EraseShaderFunction(vertexShaderText, "void FSmain()");
        
    if(version.compare("330") == 0)
    {
        String_ReplaceAll(vertexShaderText, "attribute", "layout(location = %d) in", "", 0, true);
        String_ReplaceAll(vertexShaderText, "varying", "out");
    }

    return CreateShader(vertexShaderText, GL_VERTEX_SHADER);
	//std::string vertexShaderText = "#version 330\n\n";
	//std::string token = "";
	//std::string lastToken = "";
	//size_t tokenLocation = String_NextToken(shaderText, 0, &token);

	//bool appendNext = false;
	//int attributeCounter = 0;
	//std::string nextAppendEnding = "";
	//std::string nextAppendBefore = "";
	//std::string appendBefore = "";

	//while(tokenLocation != 0)
	//{
	//	if(appendNext)
	//	{
	//		appendNext = false;
	//		vertexShaderText += (nextAppendBefore + token + nextAppendEnding);
	//		continue;
	//	}

	//	if(token.compare("cbuffer") == 0)
	//		appendBefore = "uniform ";
	//	if(token.compare("}") == 0)
	//	{
	//		nextAppendBefore = "";
	//		appendBefore = "";
	//	}

	//	if(lastToken.compare("struct") == 0 && token.compare("VS_IN") == 0)
	//	{
	//		appendBefore = "layout (location = 0) in ";
	//		nextAppendBefore = "VS_IN_";
	//	}

	//	if(lastToken.compare("struct") == 0 && token.compare("PS_IN") == 0)
	//	{
	//		appendBefore = "out ";
	//		nextAppendBefore = "";
	//	}

	//	if(token.compare("Texture2D") == 0)
	//	{
	//		appendNext = true;
	//		vertexShaderText += "uniform sampler2D ";
	//		nextAppendEnding = ";\n";
	//	}

	//	if(token.compare("matrix") == 0)
	//	{
	//		appendNext = true;
	//		vertexShaderText += (appendBefore + "mat4 ");
	//		nextAppendEnding = ";\n";
	//	}

	//	if(token.compare("float4") == 0)
	//	{
	//		appendNext = true;
	//		vertexShaderText += (appendBefore + "vec4 ");
	//		nextAppendEnding = ";\n";
	//	}

	//	if(token.compare("float2") == 0)
	//	{
	//		appendNext = true;
	//		vertexShaderText += (appendBefore + "vec2 ");
	//		nextAppendEnding = ";\n";
	//	}

	//	//vertexShaderText += token;
	//	//vertexShaderText += " ";

	//	//if(token.compare(";") == 0)
	//	//	vertexShaderText += "\n";
	//	//if(lastToken.compare("}") == 0)
	//	//	vertexShaderText += "\n";

	//	lastToken = token;
	//	tokenLocation = String_NextToken(shaderText, tokenLocation, &token);
	//	std::cout << token << std::endl;
	//}

	//std::cout << vertexShaderText << std::endl;

	//return 0;
}

#include <iostream>

unsigned int Renderer::CreateFragmentShader(const std::string& shaderText)
{
	std::string version = GetSpecifiedVersion(shaderText);
    
    size_t vertexShaderKeyLocation = shaderText.find("void VSmain()");
    size_t fragmentShaderKeyLocation = shaderText.find("void FSmain()");

    std::string fragmentShaderText = std::string(shaderText);
    String_FindAndReplace(fragmentShaderText, "void FSmain()", "void main()");
        
    if(vertexShaderKeyLocation != std::string::npos)
            EraseShaderFunction(fragmentShaderText, "void VSmain()");
        
    String_ReplaceAll(fragmentShaderText, "attribute", "", "\n");
        
    if(version.compare("330") == 0)
	{
        String_ReplaceAll(fragmentShaderText, "varying", "in");
		String_FindAndReplace(fragmentShaderText, "gl_FragColor", "OUT_Fragment_Color");

		std::string newFragout = "out vec4 OUT_Fragment_Color;\n";
		size_t start = fragmentShaderText.find("\n");
		fragmentShaderText.replace(start + 1,0,newFragout);
	}

    return CreateShader(fragmentShaderText, GL_FRAGMENT_SHADER);
}

unsigned int Renderer::CreateShaderProgram(unsigned int* shaders, int numShaders)
{
    unsigned int program = glCreateProgram();
    
    for(int i = 0; i < numShaders; i++)
        glAttachShader(program, shaders[i]);
    
    glLinkProgram(program);
    CheckShaderError(program, GL_LINK_STATUS, true, "Error linking shader program");
    
    return program;
}

void Renderer::ValidateShaderProgram(unsigned int program)
{
    glValidateProgram(program);
    CheckShaderError(program, GL_VALIDATE_STATUS, true, "Invalid shader program");
}

void Renderer::BindShaderProgram(unsigned int program)
{
    static unsigned int lastProgram = 0;
    
    if(lastProgram != program)
    {
        glUseProgram(program);
        lastProgram = program;
    }
}

unsigned int Renderer::GetShaderProgramUniformLocation(unsigned int program, const std::string& name)
{
    return glGetUniformLocation(program, name.c_str());
}

void Renderer::DeleteShaderProgram(unsigned int program, unsigned int* shaders, int numShaders)
{
    for(int i = 0; i < numShaders; i++)
    {
        glDetachShader(program,shaders[i]);
        glDeleteShader(shaders[i]);
    }
    
    glDeleteProgram(program);
}

void Renderer::SetUniformInt(unsigned int uniformLocation, int value)
{
    glUniform1i(uniformLocation, value);
}

void Renderer::SetUniformFloat(unsigned int uniformLocation, float value)
{
    glUniform1f(uniformLocation, value);
}

void Renderer::SetUniformVector3f(unsigned int uniformLocation, const Vector3f& value)
{
    glUniform3f(uniformLocation, value.GetX(), value.GetY(), value.GetZ());
}

void Renderer::SetUniformMatrix4f(unsigned int uniformLocation, const Matrix4f& value)
{
    glUniformMatrix4fv(uniformLocation, 1, GL_TRUE, &(value[0][0]));
}

unsigned int Renderer::CreateTexture(int width, int height, unsigned char* data, bool linearFiltering, bool repeatTexture)
{
    unsigned int texture = 0;
    
    int filter;
    int wrapMode;
        
    if(linearFiltering)
        filter = GL_LINEAR;
    else
        filter = GL_NEAREST;
            
    if(repeatTexture)
        wrapMode = GL_REPEAT;
    else
        wrapMode = GL_NEAREST;
        
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
        
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode);
        
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    return texture;
}

void Renderer::DeleteTexture(unsigned int texture)
{
    glDeleteTextures(1, &texture);
}

void Renderer::BindTexture(unsigned int texture, int unit)
{
	static unsigned int lastTexture = 0;

	if(lastTexture != texture)
	{
		//NOTE: This may fail if GL_TEXTUREX is not ordered sequentially!
		assert(unit >= 0 && unit <= 31);
    
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, texture);
		lastTexture = texture;
	}
}
//
//void Renderer::CreateRenderTarget(unsigned int texture, bool depthTarget)
//{
//    unsigned int frameBuffer = glGenFramebuffers();
//    
//    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
//    glFramebufferTexture(GL_FRAMEBUFFER, attachment, id, 0);
//		
//    if(attachment == GL_DEPTH_ATTACHMENT)
//        glDrawBuffer(GL_NONE);
//    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
//    {
//        //TODO: Display Error
//    }
//		
//    if(bind)
//    {
//        lastWriteBind = frameBuffer;
//        glViewport(0,0,width,height);
//    }
//    else
//        glBindFramebuffer(GL_FRAMEBUFFER, lastWriteBind);
//}
