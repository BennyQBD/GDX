#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED

#include "gameObject.h"
#include "renderer.h"

class CoreEngine
{
public:
    CoreEngine();
    virtual void Start();
    virtual void Stop();
    virtual void Push_Game(GameObject& game);
    virtual GameObject* GetGame();
protected:
private:
    bool m_IsRunning;
    GameObject* m_pGame;
};

namespace Engine
{
    void Push_Game(GameObject& game);
	void Start();
	void Stop();
	
	GameObject* GetGame();
	RenderingComponent* GetRenderingEngine();
	Renderer* GetRenderer();
	
	void SetCoreEngine(CoreEngine& coreEngine);
	void SetRenderer(Renderer& renderer);
};

#endif // ENGINE_H_INCLUDED
