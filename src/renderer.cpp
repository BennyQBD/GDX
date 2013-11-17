#include "renderer.h"
#include "mesh.h"
#include "math3d.h"
#include "display.h"

#include <GL/glew.h>
#include <stdio.h>
#include <sstream>
#include <cassert>

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

unsigned int Renderer::CreateVertexShader(const std::string& text)
{
    return CreateShader(text, GL_VERTEX_SHADER);
}

unsigned int Renderer::CreateFragmentShader(const std::string& text)
{
    return CreateShader(text, GL_FRAGMENT_SHADER);
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
    
    if(width > 0 && height > 0 && data != 0)
    {
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
    }
    
    return texture;
}

void Renderer::DeleteTexture(unsigned int texture)
{
    glDeleteTextures(1, &texture);
}

void Renderer::BindTexture(unsigned int texture, int unit)
{
    //NOTE: This may fail if GL_TEXTUREX is not ordered sequentially!
    assert(unit >= 0 && unit <= 31);
    
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, texture);
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
