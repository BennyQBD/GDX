#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "renderingComponent.h"
#include "mesh.h"
#include "material.h"
#include "shader.h"

class MeshRenderer : public RenderingComponent
{
public:
	MeshRenderer(Mesh* pMesh = NULL, Material* pMaterial = NULL, Shader* pShader = NULL);

	virtual void Render(GameObject* pGameObject);
protected:
private:
	Mesh* m_pMesh;
	Material* m_pMaterial;
	Shader* m_pShader;
};

#endif
