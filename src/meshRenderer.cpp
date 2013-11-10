#include "meshRenderer.h"
#include "gameObject.h"

MeshRenderer::MeshRenderer(Mesh* pMesh, Material* pMaterial, Shader* pShader)
{
	m_pMesh = pMesh;
	m_pMaterial = pMaterial;
	m_pShader = pShader;
}

void MeshRenderer::Render(GameObject* pGameObject)
{
	m_pShader->Bind();
	m_pShader->Update(pGameObject->GetTransform(), *m_pMaterial);
	m_pMesh->Draw();
}
