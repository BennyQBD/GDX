#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include <string>

class Texture
{
public:
	Texture(const std::string& fileName, bool linearFiltering = true);
	Texture(int width = 0, int height = 0, unsigned char* data = 0, bool linearFiltering = true);

	~Texture();
	Texture(const Texture& texture);
	void operator=(const Texture& texture);

	void Bind(int unit = 0) const;
	bool IsInitialized() const;

protected:
private:
    static unsigned int m_LastBind;

	mutable bool m_Cleanup;
    unsigned int m_TextureID;
    
    void InitTexture(int width, int height, unsigned char* data, bool linearFiltering);
};

#endif // TEXTURE_H_INCLUDED
