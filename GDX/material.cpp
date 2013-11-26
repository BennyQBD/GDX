#include "material.h"
#include "display.h"

const std::string Material::DiffuseTextureName = "diffuse";
const std::string Material::ColorVector3fName = "color";
bool Material::HasInitialisedTextureUnits = false;

std::map<std::string, int> Material::TextureUnits = std::map<std::string, int>();
std::map<std::string, Material*> Material::Materials = std::map<std::string, Material*>();

Material* Material::Get(const std::string& name)
{
	std::map<std::string, Material*>::iterator it = Materials.find(name);
	if(it != Materials.end())
		return it->second;

	//TODO: Make this load a material from file
	return 0;
}

Material* Material::Create(const std::string& name, Texture* diffuse, const Vector3f& color)
{
	if(Materials.find(name) != Materials.end())
		Display::Error("Material " + name + " already exists, and therefore cannot be created.");

	Materials.insert(std::pair<std::string, Material*>(name, new Material(diffuse, color)));
    return Materials.at(name);
}

void Material::Delete(const std::string& name)
{
	std::map<std::string, Material*>::iterator it = Materials.find(name);
	if(it != Materials.end()) 
	{
		delete it->second;
		Materials.erase(it);
	} 
}

void Material::DeleteAll()
{
    for (std::map<std::string, Material*>::iterator it=Materials.begin(); it!=Materials.end(); ++it)
        delete it->second;
}

Material::Material(Texture* diffuse, const Vector3f& color)
{
    AddTexture(DiffuseTextureName, diffuse);
    AddVector3f(ColorVector3fName, color);
    
    if(!HasInitialisedTextureUnits)
    {
        TextureUnits.insert(std::pair<std::string,int>(DiffuseTextureName, 0));
        HasInitialisedTextureUnits = true;
    }
}
