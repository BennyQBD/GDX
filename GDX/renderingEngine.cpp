#include "renderingEngine.h"

PerspectiveCamera RenderingEngine::DefaultCamera = PerspectiveCamera(Vector3f(0, 1, -5), Vector3f::FORWARD, Vector3f::UP, ToRadians(70.0f), 0.01f, 1000.0f);

RenderingEngine::RenderingEngine(Camera* camera)
{
	m_pCamera = camera;
}

static void RenderPass(GameObject* pGameObject)
{
	for(int i = 0; i < pGameObject->GetNumberOfChildren(); i++)
    {
        GameObject* pChild = pGameObject->GetChild(i);
        
        pChild->GetTransform().SetChildModel(pGameObject->GetTransform().GetModel(false));
        pChild->Render();
        RenderPass(pChild);
    }
}

void RenderingEngine::Render(GameObject* pGameObject)
{
	Transform::CalcViewProjection(m_pCamera);
	RenderPass(pGameObject);
}

void RenderingEngine::SetCamera(Camera* pCamera)
{
	m_pCamera = pCamera;
}

Camera* RenderingEngine::GetCamera()
{
	return m_pCamera;
}