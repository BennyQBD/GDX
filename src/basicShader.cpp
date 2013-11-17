#include "basicShader.h"

BasicShader::BasicShader() : Shader("basicShader")
{
	AddUniform("transform");
    //AddUniform("color");
    ValidateShader();
}

BasicShader::~BasicShader()
{
    
}

void BasicShader::InitPass()
{
	
}

//static unsigned char whitePixel[] = {0xFF, 0xFF, 0xFF, 0xFF};

void BasicShader::Update(Transform& transform, Material& material)
{
//    static Texture WHITE = Texture(1,1,whitePixel);
//
//    if(material.texture != NULL)
//        material.texture->bind();
//    else
//        WHITE.bind();
//
    //material.texture.Bind();
    material.GetDiffuseTexture()->Bind();
    SetMatrix4f("transform", transform.GetMVP());
//    setUniform("color", material.color);
}
