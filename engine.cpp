#include "engine.h"
#include "time.h"
#include "display.h"
#include "game.h"
#include "input.h"

#include <sstream>
#include <SDL/SDL.h>

////--------------------------------------------------------------------------------------
//// Global Variables
////--------------------------------------------------------------------------------------
static const int FRAME_CAP = 500;
static bool	g_isRunning = false;

void Engine::Start()
{
	g_isRunning = true;

	Time::Init();

	const double frameLimit = 1.0/FRAME_CAP;
    int frameCount = 0;
    double frameTimeCount = 0.0;
    double timeCount = 0.0;
	double lastTime = Time::GetTime();
    bool drawFrame = false;

	Game::Init();

	while(g_isRunning)
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
            Game::Input();
            Game::Update();
				
            timeCount = timeCount - frameLimit;
        }
            
        if(drawFrame)
        {
            Display::ClearScreenAndDepth();
   
            Game::Render();

            Display::SwapBuffers();
            frameCount++;
        }
        else
        {
            SDL_Delay(1);
        }
	}
	Game::Cleanup();
}

void Engine::Stop()
{
	g_isRunning = false;
}
