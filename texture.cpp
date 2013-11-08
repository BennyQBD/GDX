#include "texture.h"
#include "stb_image.h"
#include "display.h"

GLuint Texture::m_LastBind = 0;

Texture::Texture(const std::string& fileName, GLenum textureTarget, GLfloat filter)
{
	int x, y, numComponents;
    unsigned char* data = stbi_load(("./res/textures/" + fileName).c_str(), &x, &y, &numComponents, 4);

    if(data == NULL)
        Display::Error("Unable to load texture: " + fileName);

    InitTexture(x,y,data,textureTarget,filter);
    stbi_image_free(data);
}

Texture::Texture(int width, int height, unsigned char* data, GLenum textureTarget, GLfloat filter)
{
    InitTexture(width, height, data, textureTarget, filter);
}

void Texture::InitTexture(int width, int height, unsigned char* data, GLenum textureTarget, GLfloat filter)
{
    m_TextureTarget = textureTarget;
    
    if(width > 0 && height > 0 && data != 0)
    {
        glGenTextures(1, &m_TextureID);
        glBindTexture(textureTarget, m_TextureID);
        glTexParameterf(textureTarget, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameterf(textureTarget, GL_TEXTURE_MAG_FILTER, filter);
        glTexImage2D(textureTarget, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        m_Cleanup = true;
    }
    else
    {
        m_TextureID = 0;
        m_Cleanup = false;
    }
}

Texture::~Texture()
{
    if(m_Cleanup)
        glDeleteTextures(1, &m_TextureID);
}

//TODO: Deep copy!
Texture::Texture(const Texture& texture)
{
    m_TextureTarget = texture.m_TextureTarget;
    m_TextureID = texture.m_TextureID;
    m_Cleanup = texture.m_Cleanup;
    texture.m_Cleanup = false;        
}

void Texture::operator=(const Texture& texture)
{
    m_TextureTarget = texture.m_TextureTarget;
    m_TextureID = texture.m_TextureID;
    m_Cleanup = texture.m_Cleanup;
    texture.m_Cleanup = false;   
}

void Texture::Bind(GLenum textureUnit) const
{
    if(m_LastBind != m_TextureID)
    {
        glActiveTexture(textureUnit);
        glBindTexture(m_TextureTarget, m_TextureID);
        m_LastBind = m_TextureID;
    }
}

bool Texture::IsInitialized() const
{
	return !(m_TextureTarget == 0 || m_TextureID == 0);
}
