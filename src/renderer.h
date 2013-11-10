#ifndef RENDERER_H_INCLUDED
#define RENDERER_H_INCLUDED

#include <string>
#include "math3d.h"

class Renderer
{
public:
    virtual void Init();

    virtual void SetClearColor(float r, float g, float b, float a);
    virtual void ClearScreenAndDepth();
	virtual void ClearScreen();
	virtual void ClearDepth();
	
	virtual unsigned int CreateVertexBuffer(void* data, int dataSize, bool isStatic = true);
	virtual unsigned int CreateIndexBuffer(void* data, int dataSize, bool isStatic = true);
	virtual void DrawTriangles(unsigned int vertexBuffer, unsigned int indexBuffer, int nIndices);
	virtual void DeleteBuffer(unsigned int buffer);
	
    virtual unsigned int CreateVertexShader(const std::string& text);
    virtual unsigned int CreateFragmentShader(const std::string& text);
    virtual unsigned int CreateShaderProgram(unsigned int* shaders, int numShaders);
    virtual unsigned int GetShaderProgramUniformLocation(unsigned int program, const std::string& name);
    virtual void ValidateShaderProgram(unsigned int program);
    virtual void BindShaderProgram(unsigned int program);
    virtual void DeleteShaderProgram(unsigned int program, unsigned int* shaders, int numShaders);
    
    virtual void SetUniformInt(unsigned int uniformLocation, int value);
    virtual void SetUniformFloat(unsigned int uniformLocation, float value);
    virtual void SetUniformVector3f(unsigned int uniformLocation, const Vector3f& value);
    virtual void SetUniformMatrix4f(unsigned int uniformLocation, const Matrix4f& value);
    
    virtual unsigned int CreateTexture(int width, int height, unsigned char* data, bool linearFiltering = true);
    virtual void BindTexture(unsigned int texture, int unit);
    virtual void DeleteTexture(unsigned int texture);
protected:
private:
};

#endif // RENDERER_H_INCLUDED
