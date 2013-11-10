#ifndef MATERIAL_H_INCLUDED
#define MATERIAL_H_INCLUDED

#include "texture.h"
#include "math3d.h"

struct Material
{
	Texture texture;
	Quaternion color;
};

#endif // MATERIAL_H_INCLUDED
