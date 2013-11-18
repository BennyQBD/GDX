#ifndef MATERIAL_H_INCLUDED
#define MATERIAL_H_INCLUDED

#include "texture.h"
#include "math3d.h"
#include <map>
#include <string>

class Material
{
public:
    //static Material DEFAULT;
    static const std::string DiffuseTextureName;
    static const std::string ColorVector3fName;
    
    Material(const Texture& diffuse = DefaultTextureParameter, const Vector3f& color = Vector3f(1,1,1));
    
    inline void AddTexture(const std::string& name, const Texture& texture)    {m_TextureMap.insert(std::pair<std::string,const Texture*>(name, &texture));}
    inline void AddVector3f(const std::string& name, const Vector3f& vector3f) {m_Vector3fMap.insert(std::pair<std::string,Vector3f>(name, vector3f));}
    inline void AddFloat(const std::string& name, float value)                 {m_FloatMap.insert(std::pair<std::string,float>(name, value));}
    
    inline void RemoveTexture(const std::string& name)  {m_TextureMap.erase(name);}
    inline void RemoveVector3f(const std::string& name) {m_Vector3fMap.erase(name);}
    inline void RemoveFloat(const std::string& name)    {m_FloatMap.erase(name);}
    
    inline const Texture* GetTexture(const std::string& name)  {return m_TextureMap.at(name);}
    inline Vector3f GetVector3f(const std::string& name)       {return m_Vector3fMap.at(name);}
    inline float GetFloat(const std::string& name)            {return m_FloatMap.at(name);}
    
    inline const Texture* GetDiffuseTexture() {return GetTexture(DiffuseTextureName);}
    inline const Vector3f GetColor()          {return GetVector3f(ColorVector3fName);}
    
    inline void SetDiffuseTexture(const Texture& texture) {m_TextureMap[DiffuseTextureName] = &texture;}
    inline void SetColor(const Vector3f& color)           {m_Vector3fMap[ColorVector3fName] = color;}
    
    static inline int GetTextureUnit(const std::string& name) {return TextureUnits.at(name);}
protected:
private:
    static Texture DefaultTextureParameter;
    static bool HasInitialisedTextureUnits;
    static std::map<std::string, int> TextureUnits;
    std::map<std::string, const Texture*> m_TextureMap;
    std::map<std::string, Vector3f> m_Vector3fMap;
    std::map<std::string, float> m_FloatMap;
};

#endif // MATERIAL_H_INCLUDED
