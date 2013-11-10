#include "texture.h"
#include "stb_image.h"
#include "display.h"
#include "engine.h"

unsigned int Texture::m_LastBind = 0;

Texture::Texture(const std::string& fileName, bool linearFiltering)
{
	int x, y, numComponents;
    unsigned char* data = stbi_load(("./res/textures/" + fileName).c_str(), &x, &y, &numComponents, 4);

    if(data == NULL)
        Display::Error("Unable to load texture: " + fileName);

    InitTexture(x,y,data,linearFiltering);
    stbi_image_free(data);
}

Texture::Texture(int width, int height, unsigned char* data, bool linearFiltering)
{
    InitTexture(width, height, data, linearFiltering);
}

void Texture::InitTexture(int width, int height, unsigned char* data, bool linearFiltering)
{
    m_TextureID = Engine::GetRenderer()->CreateTexture(width, height, data, linearFiltering);
    
    if(m_TextureID != 0)
        m_Cleanup = true;
    else
        m_Cleanup = false;
}

Texture::~Texture()
{
    if(m_Cleanup)
        Engine::GetRenderer()->DeleteTexture(m_TextureID);
}

//TODO: Deep copy!
Texture::Texture(const Texture& texture)
{
    m_TextureID = texture.m_TextureID;
    m_Cleanup = texture.m_Cleanup;
    texture.m_Cleanup = false;        
}

void Texture::operator=(const Texture& texture)
{
    m_TextureID = texture.m_TextureID;
    m_Cleanup = texture.m_Cleanup;
    texture.m_Cleanup = false;   
}

void Texture::Bind(int unit) const
{
    if(m_LastBind != m_TextureID)
    {
        Engine::GetRenderer()->BindTexture(m_TextureID, unit);
        m_LastBind = m_TextureID;
    }
}

bool Texture::IsInitialized() const
{
	return !(m_TextureID == 0);
}
