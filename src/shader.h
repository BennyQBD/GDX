#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#include "display.h"
#include "math3d.h"
#include "material.h"
#include "transform.h"
#include "engine.h"

#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <iostream>

class Shader
{
public:
	Shader();
	Shader(const std::string& fileName);

	~Shader();
	Shader(const Shader& shader);
	void operator=(const Shader& shader);

	virtual void InitPass() = 0;
	virtual void Update(Transform& transform, Material& material) = 0;
	virtual void Bind();
protected:
    unsigned int m_Program;
    std::vector<unsigned int> m_Shaders;
    std::map<std::string, unsigned int> m_Uniforms;
	mutable bool m_Cleanup;
	
	void AddUniform(const std::string& uniform);
	void ValidateShader();
	
	inline std::string LoadShader(const std::string& fileName)
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
                output.append(line + "\n");
            }
        }
        else
        {
            Display::Error("Unable to load shader: " + fileName);
        }

        return output;
	}
	
	inline void SetInt(const std::string& name, int value) {Engine::GetRenderer()->SetUniformInt(m_Uniforms.at(name),value);}
    inline void SetFloat(const std::string& name, float value) {Engine::GetRenderer()->SetUniformFloat(m_Uniforms.at(name),value);}
    inline void SetVector3f(const std::string& name, const Vector3f& value) {Engine::GetRenderer()->SetUniformVector3f(m_Uniforms.at(name),value);}
    inline void SetMatrix4f(const std::string& name, const Matrix4f& value) {Engine::GetRenderer()->SetUniformMatrix4f(m_Uniforms.at(name),value);}
};

#endif // SHADER_H_INCLUDED
