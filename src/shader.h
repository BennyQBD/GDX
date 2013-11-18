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

struct UniformData
{
    unsigned int Location;
    std::string Type;
    std::string Name;
    
    UniformData(unsigned int UniformLocation, const std::string& UniformType, const std::string& UniformName)
    {
        Location = UniformLocation;
        Type = UniformType;
        Name = UniformName;
    }
};

class Shader
{
public:
	void Update(Transform& transform, Material& material);
	void Bind();
	
	static Shader* Get(const std::string& name);
    static void RemoveShaders();
private:
    static std::map<std::string, Shader*> ShaderPrograms;

    unsigned int m_Program;
    std::vector<unsigned int> m_Shaders;
    std::vector<UniformData> m_Uniforms;
    bool m_IsValidated;
	mutable bool m_Cleanup;
	
	void AddUniform(const std::string& uniform, const std::string& uniformType);
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
	
	Shader();
    Shader(const std::string& fileName);
    ~Shader();
    
    Shader(const Shader& shader) {};
	void operator=(const Shader& shader) {};
};

#endif // SHADER_H_INCLUDED
