#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#include "display.h"
#include "math3d.h"
#include "material.h"
#include "transform.h"

#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <GL/glew.h>
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
	virtual void Update(Transform& transform, const Material& material) = 0;
	virtual void Bind();
protected:
    GLuint m_Program;
    std::vector<GLuint> m_Shaders;
    std::map<std::string, GLuint> m_Uniforms;
	mutable bool		m_Cleanup;
	
	void AddShader(const std::string& text, GLuint type);
	void AddUniform(const std::string& uniform);
	void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
	void CompileShader();
	void ValidateShader();
	void CompileAndValidateShader();
	
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
	
	inline void SetInt(const std::string& name, int value) {glUniform1i(m_Uniforms.at(name), value);}
    inline void SetFloat(const std::string& name, float value) {glUniform1f(m_Uniforms.at(name), value);}
    inline void SetVector3f(const std::string& name, const Vector3f& value) {glUniform3f(m_Uniforms.at(name), value.x, value.y, value.z);}
    inline void SetMatrix4f(const std::string& name, const Matrix4f& value) {glUniformMatrix4fv(m_Uniforms.at(name), 1, GL_TRUE, &(value[0][0]));}
};

#endif // SHADER_H_INCLUDED
