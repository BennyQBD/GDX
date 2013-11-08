#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include <GL/glew.h>
#include <string>

class Texture
{
public:
	Texture(const std::string& fileName, GLenum textureTarget = GL_TEXTURE_2D, GLfloat filter = GL_LINEAR);
	Texture(int width = 0, int height = 0, unsigned char* data = 0, GLenum textureTarget = GL_TEXTURE_2D, GLfloat filter = GL_LINEAR);

	~Texture();
	Texture(const Texture& texture);
	void operator=(const Texture& texture);

	void Bind(GLenum textureUnit = GL_TEXTURE0) const;
	bool IsInitialized() const;

protected:
private:
    static GLuint m_LastBind;

	mutable bool m_Cleanup;
	GLenum m_TextureTarget;
    GLuint m_TextureID;
    
    void InitTexture(int width, int height, unsigned char* data, GLenum textureTarget, GLfloat filter);
};

#endif // TEXTURE_H_INCLUDED
