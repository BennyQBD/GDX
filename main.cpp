#include <cstdlib>
#include <SDL/SDL.h>
#include "display.h"
#include "engine.h"

int main ( int argc, char** argv )
{
    Display::Init(800, 600, "Something awesome");
    //Display::SetClearColor(1.0f,1.0f,0.0f,1.0f);
    Engine::Start();
    
    Display::Destroy();
    printf("Exited cleanly\n");
    return 0;
}
