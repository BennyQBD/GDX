#ifndef BASICSHADER_H_INCLUDED
#define BASICSHADER_H_INCLUDED

#include "shader.h"

class BasicShader : public Shader
{
public:
	static BasicShader* GetInstance()
    {
		static BasicShader instance;
		return &instance;
    }

	virtual void InitPass();
	virtual void Update(Transform& transform, Material& material);
private:
	BasicShader();
	~BasicShader();

	BasicShader(const BasicShader& shader) {} //Don't Implement
	void operator=(const BasicShader& shader) {} //Don't Implement
};

#endif // BASICSHADER_H_INCLUDED
