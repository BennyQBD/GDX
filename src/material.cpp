#include "material.h"

Texture Material::DefaultTextureParameter = Texture();
const std::string Material::DiffuseTextureName = "diffuse";
const std::string Material::ColorVector3fName = "color";

Material::Material(const Texture& diffuse, const Vector3f& color)
{
    AddTexture(DiffuseTextureName, diffuse);
    AddVector3f(ColorVector3fName, color);
}
