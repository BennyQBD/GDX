#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include <string>

class Texture
{
public:
    //TODO: Find a good way to initalize these!
    //static Texture WHITE_PIXEL;// = new Texture(1, 1, (ByteBuffer)Util.createByteBuffer(4).put(new byte[]{(byte) 0xFF,(byte) 0xFF,(byte) 0xFF,(byte) 0xFF}).flip());
	//static Texture NORMAL_UP;// = new Texture(1, 1, (ByteBuffer)Util.createByteBuffer(4).put(new byte[]{(byte) 0x80,(byte) 0x7F,(byte) 0xFF,(byte) 0xFF}).flip()); 

	Texture(const std::string& fileName, bool linearFiltering = true, bool repeatTexture = true);
	Texture(int width = 0, int height = 0, unsigned char* data = 0, bool linearFiltering = true, bool repeatTexture = true);

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
    
    void InitTexture(int width, int height, unsigned char* data, bool linearFiltering, bool repeatTexture);
};

#endif // TEXTURE_H_INCLUDED
