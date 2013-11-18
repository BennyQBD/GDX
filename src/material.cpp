#include "material.h"

Texture Material::DefaultTextureParameter = Texture();
const std::string Material::DiffuseTextureName = "diffuse";
const std::string Material::ColorVector3fName = "color";
bool Material::HasInitialisedTextureUnits = false;

std::map<std::string, int> Material::TextureUnits = std::map<std::string, int>();

Material::Material(const Texture& diffuse, const Vector3f& color)
{
    AddTexture(DiffuseTextureName, diffuse);
    AddVector3f(ColorVector3fName, color);
    
    if(!HasInitialisedTextureUnits)
    {
        TextureUnits.insert(std::pair<std::string,int>(DiffuseTextureName, 0));
        HasInitialisedTextureUnits = true;
    }
}
