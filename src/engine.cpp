#include "engine.h"
#include "timing.h"
#include "display.h"
#include "input.h"

#include <sstream>
#include <SDL/SDL.h>

static CoreEngine g_DefaultEngine;
static Renderer g_DefaultRenderer;
static CoreEngine* g_pCoreEngine = &g_DefaultEngine;
static Renderer* g_pRenderer = &g_DefaultRenderer;

void Engine::Start()
{
	g_pCoreEngine->Start();
}

void Engine::Stop()
{
	g_pCoreEngine->Stop();
}

void Engine::Push_Game(GameObject& game)
{
    g_pCoreEngine->Push_Game(game);
}

void Engine::SetCoreEngine(CoreEngine& coreEngine)
{
    g_pCoreEngine = &coreEngine;
}

void Engine::SetRenderer(Renderer& renderer)
{
    g_pRenderer = &renderer;
}

RenderingComponent* Engine::GetRenderingEngine()
{
    return Engine::GetGame()->GetRenderingComponent();
}

GameObject* Engine::GetGame()
{
    return g_pCoreEngine->GetGame();
}

Renderer* Engine::GetRenderer()
{
    return g_pRenderer;
}

////--------------------------------------------------------------------------------------
//// Core Engine
////--------------------------------------------------------------------------------------
static const int FRAME_CAP = 500;

CoreEngine::CoreEngine()
{
    static DefaultRenderingEngine defaultRenderingEngine;
    static GameObject defaultGame = GameObject(Transform(), &defaultRenderingEngine);
    m_IsRunning = false;
    m_pGame = &defaultGame;
}

void CoreEngine::Start()
{
    m_IsRunning = true;

	Time::Init();

	const double frameLimit = 1.0/FRAME_CAP;
    int frameCount = 0;
    double frameTimeCount = 0.0;
    double timeCount = 0.0;
	double lastTime = Time::GetTime();
    bool drawFrame = false;

	while(m_IsRunning)
	{
        double currentTime = Time::GetTime();
        double passedTime = currentTime - lastTime;
        frameTimeCount += passedTime;
        timeCount += passedTime;
        lastTime = currentTime;
        drawFrame = false;

        if(frameTimeCount >= 1.0)
        {
            std::stringstream  convert;
				
            convert << frameCount << ": " << 1000.0/((double)frameCount) << "ms";

            Display::SetTitle(convert.str());
            frameCount = 0;
            frameTimeCount = 0;
        }
            
        Time::SetDelta(frameLimit);
        while(timeCount >= frameLimit)
        {
            drawFrame = true;
            Input::Update();
            m_pGame->Input();
            m_pGame->Update();
				
            timeCount = timeCount - frameLimit;
        }
            
        if(drawFrame)
        {
            Engine::GetRenderer()->ClearScreenAndDepth();
   
            m_pGame->Render();

            Display::SwapBuffers();
            frameCount++;
        }
        else
        {
            SDL_Delay(1);
        }
	}
}

void CoreEngine::Stop()
{
    m_IsRunning = false;
}

void CoreEngine::Push_Game(GameObject& game)
{
    m_pGame = &game;
}

GameObject* CoreEngine::GetGame()
{
    return m_pGame;
}
