#ifndef RENDERINGENGINE_H_INCLUDED
#define RENDERINGENGINE_H_INCLUDED

#include "gameObject.h"

class RenderingEngine
{
public:
	RenderingEngine(Camera* camera = &DefaultCamera);
    virtual void Render(GameObject* pGameObject);
	void SetCamera(Camera* pCamera);
	Camera* GetCamera();
protected:
private:
	static PerspectiveCamera DefaultCamera;
	Camera* m_pCamera;
};

#endif