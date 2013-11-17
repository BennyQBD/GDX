#include "component.h"
#include "gameObject.h"
#include "mesh.h"
#include "shader.h"
#include "material.h"

RenderingComponent::RenderingComponent(Mesh* pMesh, Material* pMaterial, Shader* pShader)
{
	m_pMesh = pMesh;
	m_pMaterial = pMaterial;
	m_pShader = pShader;
}

void RenderingComponent::Render(GameObject* pGameObject)
{
	m_pShader->Bind();
	m_pShader->Update(pGameObject->GetTransform(), *m_pMaterial);
	m_pMesh->Draw();
}

void DefaultRenderingEngine::Render(GameObject* pGameObject)
{
    for(int i = 0; i < pGameObject->GetNumberOfChildren(); i++)
    {
        GameObject* pChild = pGameObject->GetChild(i);
        
        pChild->GetTransform().SetChildModel(pGameObject->GetTransform().GetModel(false));
        pChild->Render();
        Render(pChild);
    }
}

